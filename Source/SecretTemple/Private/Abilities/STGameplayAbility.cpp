// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/STGameplayAbility.h"

#include "AbilitySystemComponent.h"

USTGameplayAbility::USTGameplayAbility()
{
	//TODO Clarify
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USTGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
