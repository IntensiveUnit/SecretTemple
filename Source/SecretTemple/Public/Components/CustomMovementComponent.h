// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMovementComponent, Log, All);
/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bIsRunning;
	
	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartSprinting();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopSprinting();
	
	virtual float GetMaxSpeed() const override;
	
};
