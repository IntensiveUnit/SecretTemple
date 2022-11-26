// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CustomAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/PlayerCharacter.h"

void UCustomAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();

	LogChangedAttribute(Data);
	// Get the target character, which should be our owner
	APlayerCharacter* TargetCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();;
		TargetCharacter = Cast<APlayerCharacter>(TargetActor);
	}

	//If it was damage, we process it separately
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = false;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			if (!WasAlive)
			{
				return;
			}
			
			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		//Handle Mana changes
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
	    
		//Handle Health changes
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		//Handle Stamina changes
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	
}

void UCustomAttributeSet::LogChangedAttribute(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Attribute %s was chaged to %f"),
		*Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Magnitude);
}

