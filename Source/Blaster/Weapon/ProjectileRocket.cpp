// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"

#include "RocketMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AProjectileRocket::AProjectileRocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("RocketMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileLoop && LoopingSoundAttenuation)
	{
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(ProjectileLoop,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false,
			1.f,
			1.f,
			0.f,
			LoopingSoundAttenuation,
			(USoundConcurrency*)nullptr,
			false);
	}
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
		return;
	ExplodeDamage();
	StartDestroyTimer();
	PlayImpactParticles();
	PlayImpactSound(Hit.Location);
	if(ProjectileMesh)
		ProjectileMesh->SetVisibility(false);
	if(CollisionBox)
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
		ProjectileLoopComponent->Stop();
	
}