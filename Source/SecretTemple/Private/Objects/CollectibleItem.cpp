// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/CollectibleItem.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"


void ACollectibleItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FCollectibleItemData* CacheCollectibleItemData = LoadData<FCollectibleItemData>();
	
	if (!CacheCollectibleItemData)
	{
		UE_LOG(LogItems, Error, TEXT("Failed to load data from DataTable in %s"), *this->GetName());
		return;
	}
	
	SetCollectibleItemData(CacheCollectibleItemData);
}

void ACollectibleItem::SetCollectibleItemData(const FCollectibleItemData& InCollectibleItemData)
{
	CollectibleItemData = InCollectibleItemData;

	if (!CollectibleItemData.StaticMesh)
	{
		UE_LOG(LogItems, Warning, TEXT("No static mesh in item %s"), *CollectibleItemData.Name.ToString());
		return;
	}
	
	StaticMeshComponent->SetStaticMesh(CollectibleItemData.StaticMesh);
}

FCollectibleItemData& ACollectibleItem::GetCollectibleItemData()
{
	return CollectibleItemData;
}

TEnumAsByte<EItemType> ACollectibleItem::GetItemType()
{
	return EItemType::Collectible;
}

bool ACollectibleItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventoryComponent()->AddCollectibleItem(this);

	if (bWasAdded)
	{
		OnItemWasPickedUp.Broadcast(this, InteractionInstigator);
	}
	
	return bWasAdded;
}




