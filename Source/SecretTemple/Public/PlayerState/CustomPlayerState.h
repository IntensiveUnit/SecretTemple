// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

class UCustomAttributeSet;
/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACustomPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCustomAttributeSet* GetAttributeSetBase() const;

	
protected:
	UPROPERTY(EditDefaultsOnly)
	UAbilitySystemComponent* GameplayAbilitySystemComponent;

	UPROPERTY()
	UCustomAttributeSet* AttributeSet;
	
};
