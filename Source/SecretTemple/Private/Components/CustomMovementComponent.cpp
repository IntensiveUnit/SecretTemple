// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"

#include "Abilities/CustomAttributeSet.h"
#include "Character/PlayerCharacter.h"

DEFINE_LOG_CATEGORY(LogMovementComponent);

float UCustomMovementComponent::GetMaxSpeed() const
{
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner)
	{
		return Super::GetMaxSpeed();
	}
	
	if (IsCrouching())
	{
		UE_LOG(LogMovementComponent, Log, TEXT("Is crouching, speed is: %f"), Owner->GetCrouchingSpeed());
		return Owner->GetCrouchingSpeed();
	}
	
	if (bIsRunning)
	{
		return Owner->GetRunningSpeed();
	}
	
	return Owner->GetWalkingSpeed();
}

void UCustomMovementComponent::StartSprinting()
{
	bIsRunning = true;
}

void UCustomMovementComponent::StopSprinting()
{
	bIsRunning = false;
}






