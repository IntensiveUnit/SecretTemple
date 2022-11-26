// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/NoteItem.h"

#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"

void ANoteItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FNoteItemData* CacheNoteItemData = LoadData<FNoteItemData>();
	
	if (!CacheNoteItemData)
	{
		UE_LOG(LogItems, Error, TEXT("Failed to load data from DataTable in %s"), *this->GetName());
		return;
	}
	 
	SetNoteItemData(CacheNoteItemData);
}

void ANoteItem::SetNoteItemData(const FNoteItemData& InNoteItemData)
{
	if (!InNoteItemData.StaticMesh)
	{
		UE_LOG(LogItems, Warning, TEXT("No static mesh in item %s"), *InNoteItemData.Name.ToString());
		return;
	}
	
	StaticMeshComponent->SetStaticMesh(InNoteItemData.StaticMesh);
	
	NoteItemData = InNoteItemData;
}

FNoteItemData& ANoteItem::GetNoteItemData()
{
	return NoteItemData;
}

TEnumAsByte<EItemType> ANoteItem::GetItemType()
{
	return EItemType::Note;
}

bool ANoteItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventoryComponent()->AddNoteItem(this);

	if (bWasAdded)
	{
		OnItemWasPickedUp.Broadcast(this, InteractionInstigator);
	}
	
	return bWasAdded;
}
