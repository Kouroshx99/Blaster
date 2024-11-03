// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

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
		FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		UWorld * World = GetWorld();
		if(World)
		{
			World->LineTraceSingleByChannel(FireHit,
				Start,
				End,
				ECC_Visibility);
			if(FireHit.bBlockingHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("ImpactParticles"));
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
						FireHit.Location,
						FireHit.ImpactNormal.Rotation());
				}
			}
		}
	}
}
