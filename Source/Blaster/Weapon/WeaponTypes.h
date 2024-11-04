#pragma once

#define TRACE_LENGHT 80000;

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SubMachineGun UMETA(DisplayName = "SunMachineGun"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "SniperRifle"),

	EWT_MAX UMETA(DisplayName = "DefaultMax")
};