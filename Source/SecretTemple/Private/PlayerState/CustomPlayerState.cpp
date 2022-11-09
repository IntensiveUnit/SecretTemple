// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/CustomPlayerState.h"
#include "Abilities/CustomAttributeSet.h"

ACustomPlayerState::ACustomPlayerState()
{
	GameplayAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GameplayAbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACustomPlayerState::GetAbilitySystemComponent() const
{
	return GameplayAbilitySystemComponent;
}

UCustomAttributeSet* ACustomPlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}



