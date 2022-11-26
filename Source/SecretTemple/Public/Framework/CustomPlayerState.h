// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

class UAttributeSet;
class UCustomAttributeSet;
/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	ACustomPlayerState();
	

	//------------------------------------------------------------------//
	//----------------------Implement IAbilitySystemInterface-----------//
	//------------------------------------------------------------------//
	
public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCustomAttributeSet* GetAttributeSet() const;
	
	//------------------------------------------------------------------//
	//----------------------Properties----------------------------------//
	//------------------------------------------------------------------//

private:
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UCustomAttributeSet* AttributeSet;


	
};
