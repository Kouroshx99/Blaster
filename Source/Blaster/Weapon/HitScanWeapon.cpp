// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn)
		return;
	AController* InstigatorController = OwnerPawn->GetInstigatorController();
	if(MuzzleSocket && InstigatorController)
	{
		FTransform MuzzleSocketTransform = MuzzleSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = MuzzleSocketTransform.GetLocation();
		
		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		UWorld* World = GetWorld();

		if(FireHit.bBlockingHit)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if(BlasterCharacter)
			{
				UGameplayStatics::ApplyDamage(BlasterCharacter,
					Damage,
					InstigatorController,
					this,
					UDamageType::StaticClass());
					
			}
			if(ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World,
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			}

			if(HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this,
					HitSound,
					FireHit.ImpactPoint);
			}
				
		}

		if(MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World,
				MuzzleFlash,
				MuzzleSocketTransform);
		}
		if(FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,
				FireSound,
				MuzzleSocketTransform.GetLocation());
		}
	}
}

FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector DirectionToTarget = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + DirectionToTarget * DistanceToSphere;
	FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SohereRadius);
	FVector EndLoc = SphereCenter + RandVec;
	FVector ToEndLoc = EndLoc - TraceStart;

	
	/*DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
	DrawDebugSphere(GetWorld(),SphereCenter, SohereRadius, 12, FColor::Red, true);
	DrawDebugLine(GetWorld(),
		TraceStart,
		TraceStart + 80000.f * ToEndLoc / ToEndLoc.Size(),
		FColor::Cyan,
		true);
	*/
	return FVector(TraceStart + 80000.f * ToEndLoc / ToEndLoc.Size());
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	FHitResult FireHit;
	UWorld* World = GetWorld();
	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");

	if(World)
	{
		FVector End = bUseScatter? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25f;
		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECC_Visibility
			);
		FVector BeamEnd = End;
		if(OutHit.bBlockingHit)
			BeamEnd = OutHit.ImpactPoint;
		if(BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator);
			if(Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}