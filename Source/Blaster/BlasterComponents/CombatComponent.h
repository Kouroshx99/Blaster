// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGHT 80000;

enum class ECombatState : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ABlasterCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	void UpdateAmmoValues();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);
	void Fire();

	void FireButtonPressed(bool bPressed);
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);
	int32 AmountToReload();

private:
	class ABlasterCharacter* Character;
	class ABlasterPlayerController* Controller;
	class ABlasterHUD* HUD;
	class AWeapon* EquippedWeapon;

	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FHUDPackage HUDPackage;

	FVector HitTarget;

	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category= Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category= Combat)
	float ZoomedInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FinishFireTimer();

	bool CanFire();
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 200;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 60;
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 20;
	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 30;
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 60;
	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 0;
	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo = 10;
	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo = 5;

	void InitializeCarriedAmmo();

	ECombatState CombatState;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void HandleReload();

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
};