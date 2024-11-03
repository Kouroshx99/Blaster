// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
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
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		UWorld * World = GetWorld();
		if(World)
		{
			World->LineTraceSingleByChannel(FireHit,
				Start,
				End,
				ECC_Visibility);
			FVector BeamEnd = End;
			if(FireHit.bBlockingHit)
			{
				BeamEnd = FireHit.ImpactPoint;
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
			if(BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World,
					BeamParticles,
					MuzzleSocketTransform);
				if(Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
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

	DrawDebugSphere(GetWorld(),SphereCenter, SohereRadius, 12, FColor::Red, true);

	return FVector::ZeroVector;
}
