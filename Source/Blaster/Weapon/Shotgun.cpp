// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

void AShotgun::Fire(const FVector& HitTarget)
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
		uint32 Hits = 0;
		ABlasterCharacter* BlasterCharacter;
		TMap<ABlasterCharacter*, uint32> HitMap;
		for(uint32 i = 0; i < NumberOfPallets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if(BlasterCharacter)
			{
				if(!HitMap.Contains(BlasterCharacter))
					HitMap.Add(BlasterCharacter, 0);
				HitMap[BlasterCharacter]++;
			}
			if(ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			}

			if(HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this,
					HitSound,
					FireHit.ImpactPoint,
					0.5f,
					FMath::FRandRange(-0.5f, 0.5f));
			}
		}

		for(TPair<ABlasterCharacter*, uint32> HitPair : HitMap)
		{
			if(InstigatorController)
			{
				UGameplayStatics::ApplyDamage(HitPair.Key,
					Damage * HitPair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass());
			}
		}
	}
}