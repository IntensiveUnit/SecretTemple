// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STMovementComponent.h"

#include "Character/STCharacter.h"
#include "PlayerState/STPlayerState.h"



float USTMovementComponent::GetMaxSpeed() const
{
	ASTCharacter* Owner = Cast<ASTCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	ASTPlayerState* PlayerState = Cast<ASTPlayerState>(Owner->GetPlayerState());
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No PlayerState"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}
	
	USTAttributeSet* AttributeSet = PlayerState->GetAttributeSetBase();
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No AttributeSetBase"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}


	if (IsCrouching())
	{
		return MaxWalkSpeedCrouched;
	}
	
	if (bIsRunning)
	{
		return AttributeSet->GetRunningSpeed();
	}
	
	return AttributeSet->GetWalkingSpeed();
}

void USTMovementComponent::StartSprinting()
{
	bIsRunning = true;
}

void USTMovementComponent::StopSprinting()
{
	bIsRunning = false;
}




