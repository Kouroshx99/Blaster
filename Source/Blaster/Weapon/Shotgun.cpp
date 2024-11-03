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
		FVector End = TraceEndWithScatter(Start, HitTarget);
		
		
		
	}
}
