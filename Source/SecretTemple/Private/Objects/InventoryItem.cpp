// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/InventoryItem.h"

#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "PlayerState/CustomPlayerState.h"

bool AInventoryItem::CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	return true;
}

bool AInventoryItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventory()->AddItem(FInventoryItemInfo(this, FItemCoordinate(0,0)));

	if (bWasAdded)
	{
		HideItem();
	}
	
	return bWasAdded;
}

bool AInventoryItem::UseItem(APlayerCharacter* InteractionInstigator)
{
	if (!ItemInfo.ItemEffect) { return false; }

	if (!InteractionInstigator){ return false; }
	
	if (!InteractionInstigator->GetPlayerState()) { return false;}
	
	const ACustomPlayerState* PlayerState = InteractionInstigator->GetPlayerState<ACustomPlayerState>();
	if (!PlayerState) { return false; }

	UAbilitySystemComponent* GameplayAbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	if (!GameplayAbilitySystemComponent) { return false; }
	
	GameplayAbilitySystemComponent->ApplyGameplayEffectToSelf(ItemInfo.ItemEffect.GetDefaultObject(), 1, GameplayAbilitySystemComponent->MakeEffectContext());

	Destroy();
	
	return true;
}







