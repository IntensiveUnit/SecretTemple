// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SecretTemple/SecretTemple.h"
#include "CustomGameplayAbility.generated.h"

/**
 * Custom gameplay ability, is used to make it more convenient to be able to assign an input
 */
UCLASS()
class SECRETTEMPLE_API UCustomGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
};
