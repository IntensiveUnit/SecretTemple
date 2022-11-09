// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CustomAttributeSet.generated.h"

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
class SECRETTEMPLE_API UCustomAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	
	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, ManaRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, HealthRegen)
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Stamina)

	// MaxStamina is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxStamina)
	
	// Stamina regen rate will passively increase Stamina every second
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StaminaRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData WalkingSpeed;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, WalkingSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData RunningSpeed;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, RunningSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StandardJumpHeight;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StandardJumpHeight)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StandardJumpLength;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StandardJumpLength)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData BoostJumpHeight;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, BoostJumpHeight)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData BoostJumpLength;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, BoostJumpLength)
	

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
};
