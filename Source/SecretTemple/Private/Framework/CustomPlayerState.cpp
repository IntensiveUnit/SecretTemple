// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CustomPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Abilities/CustomAttributeSet.h"


ACustomPlayerState::ACustomPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACustomPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCustomAttributeSet* ACustomPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
