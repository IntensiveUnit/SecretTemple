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
		//UE_LOG(LogMovementComponent, Error, TEXT("No Owner"));
		return Super::GetMaxSpeed();
	}
	
	if (IsCrouching())
	{
		//UE_LOG(LogMovementComponent, Log, TEXT("Is crouching, speed is: %f"), MaxWalkSpeedCrouched);
		return MaxWalkSpeedCrouched;
	}
	
	if (bIsRunning)
	{
		//UE_LOG(LogMovementComponent, Log, TEXT("Is running, speed is: %f"), Owner->GetRunningSpeed());
		return Owner->GetRunningSpeed();
	}
	
	//UE_LOG(LogMovementComponent, Log, TEXT("Is walking, speed is: %f"), Owner->GetWalkingSpeed());
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






