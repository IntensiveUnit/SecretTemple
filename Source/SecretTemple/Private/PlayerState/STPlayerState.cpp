// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/STPlayerState.h"

ASTPlayerState::ASTPlayerState()
{
	GameplayAbilitySystemComponent = CreateDefaultSubobject<USTAbilitySystemComponent>(TEXT("GameplayAbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USTAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ASTPlayerState::GetAbilitySystemComponent() const
{
	return GameplayAbilitySystemComponent;
}

USTAttributeSet* ASTPlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}



