// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/SpringArmComponent.h"


ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom  =CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera =CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation  = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(PlayerInputComponent)
	{

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
		
		PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
		PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
		PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	}
}

void ABlasterCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		if (GEngine)
		{
			FVector StartLocation = GetActorLocation();
			FVector EndLocation = StartLocation + (Direction * 200.0f);  // Length of the line/arrow

			// Draw a red line in the direction of movement
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.1f, 0, 5.0f);

			// Optionally, draw an arrow instead of a line
			DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 50.0f, FColor::Red, false, 0.1f, 0, 5.0f);
		}

		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		if (GEngine)
		{
			FVector StartLocation = GetActorLocation();
			FVector EndLocation = StartLocation + (Direction * 200.0f);  // Length of the line/arrow

			// Draw a red line in the direction of movement
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.1f, 0, 5.0f);

			// Optionally, draw an arrow instead of a line
			DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 50.0f, FColor::Green, false, 0.1f, 0, 5.0f);
		}

		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}