// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class BLASTER_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileBullet();

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	virtual void Tick(float DeltaTime) override;
};
