// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "PlayerState/CustomPlayerState.h"


float UCustomMovementComponent::GetMaxSpeed() const
{
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	ACustomPlayerState* PlayerState = Cast<ACustomPlayerState>(Owner->GetPlayerState());
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No PlayerState"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}
	
	UCustomAttributeSet* AttributeSet = PlayerState->GetAttributeSetBase();
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

void UCustomMovementComponent::StartSprinting()
{
	bIsRunning = true;
}

void UCustomMovementComponent::StopSprinting()
{
	bIsRunning = false;
}






