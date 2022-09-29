// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/STAbilitySystemComponent.h"
#include "Abilities/STAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "STPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API ASTPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASTPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	USTAttributeSet* GetAttributeSetBase() const;

	
protected:
	UPROPERTY(EditDefaultsOnly)
	USTAbilitySystemComponent* GameplayAbilitySystemComponent;

	UPROPERTY()
	USTAttributeSet* AttributeSet;
	
};
