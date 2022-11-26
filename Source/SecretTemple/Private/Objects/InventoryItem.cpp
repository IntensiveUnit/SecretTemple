// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/InventoryItem.h"

#include "AbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogInventory)

void AInventoryItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FInventoryItemData* CacheInventoryItemData = LoadData<FInventoryItemData>();
	
	//If the data is empty after LoadData, then is error
	if (!CacheInventoryItemData)
	{
		UE_LOG(LogItems, Error, TEXT("Failed to load data from DataTable in %s"), *this->GetName());
		return;
	}
	
	SetInventoryItemData(CacheInventoryItemData);
}

void AInventoryItem::SetInventoryItemData(const FInventoryItemData& InInventoryItemData)
{
	InventoryItemData = InInventoryItemData;

	if (!InventoryItemData.StaticMesh)
	{
		UE_LOG(LogItems, Warning, TEXT("No static mesh in item %s"), *InventoryItemData.Name.ToString());
		return;
	}
	
	StaticMeshComponent->SetStaticMesh(InventoryItemData.StaticMesh);
}

FInventoryItemData& AInventoryItem::GetInventoryItemData()
{
	return InventoryItemData;
}

TEnumAsByte<EItemType> AInventoryItem::GetItemType()
{
	return EItemType::Inventory;
}

bool AInventoryItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventoryComponent()->AddInventoryItem(this);
	
	return bWasAdded;
}

bool AInventoryItem::UseItem(APlayerCharacter* InteractionInstigator)
{
	if (!InventoryItemData.ItemEffect) { return false; }

	if (!InteractionInstigator){ return false; }
	
	if (!InteractionInstigator->GetPlayerState()) { return false;}
	
	UAbilitySystemComponent* GameplayAbilitySystemComponent = InteractionInstigator->GetAbilitySystemComponent();
	if (!GameplayAbilitySystemComponent) { return false; }
	
	GameplayAbilitySystemComponent->ApplyGameplayEffectToSelf(InventoryItemData.ItemEffect.GetDefaultObject(), 1, GameplayAbilitySystemComponent->MakeEffectContext());

	//TODO what about amount in items?
	Destroy();
	
	return true;
}







