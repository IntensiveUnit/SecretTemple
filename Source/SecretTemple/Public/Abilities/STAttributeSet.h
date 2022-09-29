// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "STAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

UCLASS()
class SECRETTEMPLE_API USTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	
	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, ManaRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, HealthRegen)
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, Stamina)

	// MaxStamina is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, MaxStamina)
	
	// Stamina regen rate will passively increase Stamina every second
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, StaminaRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData WalkingSpeed;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, WalkingSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData RunningSpeed;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, RunningSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StandardJumpHeight;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, StandardJumpHeight)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StandardJumpLength;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, StandardJumpLength)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData BoostJumpHeight;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, BoostJumpHeight)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData BoostJumpLength;
	ATTRIBUTE_ACCESSORS(USTAttributeSet, BoostJumpLength)
	

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
};
