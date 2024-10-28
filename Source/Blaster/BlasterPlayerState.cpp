// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"

#include "Character/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr? Cast<ABlasterCharacter>(GetPawn()) : Character;

	if(Character)
		Controller = Controller == nullptr? Cast<ABlasterPlayerController>(Character->Controller): Controller;
	
	if(Controller)
		Controller->SetHUDScore(GetScore());
}

void ABlasterPlayerState::AddScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	Character = Character == nullptr? Cast<ABlasterCharacter>(GetPawn()) : Character;

	if(Character)
		Controller = Controller == nullptr? Cast<ABlasterPlayerController>(Character->Controller): Controller;
	
	if(Controller)
		Controller->SetHUDScore(GetScore());
}

void ABlasterPlayerState::AddToDefeats()
{
	Defeats += 1;

	Character = Character == nullptr? Cast<ABlasterCharacter>(GetPawn()) : Character;

	if(Character)
		Controller = Controller == nullptr? Cast<ABlasterPlayerController>(Character->Controller): Controller;
	
	if(Controller)
		Controller->SetHUDDefeats(Defeats);
}
