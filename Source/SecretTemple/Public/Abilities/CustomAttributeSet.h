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
 * Custom attribute set
 */

UCLASS()
class SECRETTEMPLE_API UCustomAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	/**
	* Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Damage)

	/**
	* Used for special abilities
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Mana)

	/**
	* Used for special abilities
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxMana)

	/**
	* Used by entities that contain an gameplay effect that restores mana (consumable items, etc.)
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, ManaRegen)

	/**
	* The value of hit points, when reaching zero the character dies
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Health)

	/**
	* Maximum hit points a character can have
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxHealth)

	/**
	* Used by entities that contain an gameplay effect that restores health (consumable items, etc.)
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, HealthRegen)

	/**
	* A value that determines whether the character can run
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Stamina)

	/**
	* A value that determines whether the character can run
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxStamina)
	
	/**
	* How much stamina is regenerated per second, applied to the character as an effect
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StaminaRegen)

	/**
	* Character's walking speed
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData WalkingSpeed;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, WalkingSpeed)

	/**
	* Character's running speed
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData RunningSpeed;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, RunningSpeed)

	/**
	* Character's jump height when walking
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData WalkingJumpHeight;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, WalkingJumpHeight)

	/**
	* Character's jump length when walking
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData WalkingJumpLength;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, WalkingJumpLength)

	/**
	* Character's jump height when running
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData RunningJumpHeight;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, RunningJumpHeight)

	/**
	* Character's jump length when running
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData RunningJumpLength;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, RunningJumpLength)


	static void LogChangedAttribute(const FGameplayEffectModCallbackData& Data);

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	
	
};
