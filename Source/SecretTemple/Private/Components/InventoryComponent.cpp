// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/CollectibleItem.h"
#include "Objects/InventoryItem.h"

UInventoryComponent::UInventoryComponent():
bIsSimulatedInventoryDirty(true),
InventorySize(7, 7)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	
	for (const auto& DataTableRowHandle : StartingInventoryItems)
	{
		if (!DataTableRowHandle.DataTable || DataTableRowHandle.RowName.IsNone()) { continue; }
		
		FInventoryItemData InventoryItemData = DataTableRowHandle.DataTable->FindRow<FInventoryItemData>(DataTableRowHandle.RowName, "");
		
		if (InventoryItemData.IsEmpty()){ continue; }
		
		AddInventoryItem(SpawnInventoryItem(InventoryItemData));
	}
	
	for (const auto& DataTableRowHandle : StartingCollectibleItems)
	{
		if (!DataTableRowHandle.DataTable || DataTableRowHandle.RowName.IsNone()) { continue; }
		
		FCollectibleItemData InventoryItemData = DataTableRowHandle.DataTable->FindRow<FCollectibleItemData>(DataTableRowHandle.RowName, "");
		
		if (InventoryItemData.DataIsEmpty()){ continue; }

		ACollectibleItem* CollectibleItem = SpawnCollectibleItem(InventoryItemData);
		
		AddCollectibleItem(CollectibleItem);
	}

	for (const auto& DataTableRowHandle : StartingNoteItems)
	{
		if (!DataTableRowHandle.DataTable || DataTableRowHandle.RowName.IsNone()) { continue; }
		
		FNoteItemData NoteItemData = DataTableRowHandle.DataTable->FindRow<FNoteItemData>(DataTableRowHandle.RowName, "");
		
		if (NoteItemData.DataIsEmpty()){ continue; }

		ANoteItem* NoteItem = SpawnNoteItem(NoteItemData);
		
		AddNoteItem(NoteItem);
	}
}

bool UInventoryComponent::AddInventoryItem(AInventoryItem* InInventoryItem)
{
	if (!CheckItemReference(InInventoryItem)) { return false; }
	
	const FInventoryItemData ItemData = InInventoryItem->GetInventoryItemData();

	if (!CheckInventoryItemData(ItemData)) { return false; }

	if (!CheckInventoryItemSize(ItemData)) { return false; }
	
	FItemCoordinate Coordinate;
	if (CanFitItemSomewhere(InInventoryItem,Coordinate))
	{
		AddInventoryItem_Internal(FInventoryItemSlot(InInventoryItem, Coordinate));
		
		return true;
	}
	else
	{
		UE_LOG(LogInventory, Warning, TEXT("Can't add inventory item, no space"))

		return false;
	}
	
}

bool UInventoryComponent::AddInventoryItemAtCoordinate(const FInventoryItemSlot& InInventoryItemSlot)
{
	if (!CheckItemReference(InInventoryItemSlot.InventoryItem)) { return false; }
	
	const FInventoryItemData ItemData = InInventoryItemSlot.InventoryItem->GetInventoryItemData();

	if (!CheckInventoryItemData(ItemData)) { return false; }

	if (!CheckInventoryItemSize(ItemData)) { return false; }

	if (!CheckInventoryItemCoordinate(InInventoryItemSlot.Coordinate)) {return false; }
	
	if (CanFitItemAtCoordinate(InInventoryItemSlot))
	{
		AddInventoryItem_Internal(InInventoryItemSlot);
		
		return true;
	}
	else
	{
		UE_LOG(LogInventory, Warning, TEXT("Can't add inventory item at coordinate, no space"))
		
		return false;
	}
}

void UInventoryComponent::AddInventoryItem_Internal(const FInventoryItemSlot& InInventoryItemSlot)
{
	InventoryItems.AddUnique(InInventoryItemSlot);

	bIsSimulatedInventoryDirty = true;
	
	if(OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }
	
	if(OnInventoryItemAdded.IsBound()) { OnInventoryItemAdded.Broadcast(InInventoryItemSlot.InventoryItem); }

	InInventoryItemSlot.InventoryItem->SetActorLocation(FVector(0,0,-9999.f));
	
	const FInventoryItemData ItemData = InInventoryItemSlot.InventoryItem->GetInventoryItemData();
	
	UE_LOG(LogInventory, Warning, TEXT("Inventory item %s added"), *ItemData.Name.ToString())
	
}

bool UInventoryComponent::AddCollectibleItem(ACollectibleItem* InCollectibleItem)
{
	if (!CheckItemReference(InCollectibleItem)) { return false; }
	
	CollectibleItems.AddUnique(InCollectibleItem);

	if (OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }
	
	if (OnCollectibleItemAdded.IsBound()) { OnCollectibleItemAdded.Broadcast(InCollectibleItem); }

	InCollectibleItem->SetActorLocation(FVector(0,0,-9999.f));

	UE_LOG(LogInventory, Warning, TEXT("Collectible item %s added"), &InCollectibleItem->GetCollectibleItemData().Name)
	
	return true;
}

bool UInventoryComponent::AddNoteItem(ANoteItem* InNoteItem)
{
	if (!CheckItemReference(InNoteItem)) { return false; }
	
	NoteItems.AddUnique(InNoteItem);

	if (OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }
	
	if (OnNoteItemAdded.IsBound()) { OnNoteItemAdded.Broadcast(InNoteItem); }

	InNoteItem->SetActorLocation(FVector(0,0,-9999.f));

	UE_LOG(LogInventory, Warning, TEXT("Note item %s added"), &InNoteItem->GetNoteItemData().Name)
	
	return true;
}

bool UInventoryComponent::RemoveInventoryItem(AInventoryItem* InInventoryItem)
{
	if (!CheckItemReference(InInventoryItem)) { return false; }
	
	const FInventoryItemData ItemData = InInventoryItem->GetInventoryItemData();

	if (!CheckInventoryItemData(ItemData)) { return false; }
	
	for (const auto &InventoryItem : InventoryItems)
	{
		if (InventoryItem.InventoryItem->GetInventoryItemData() == ItemData)
		{
			RemoveInventoryItem_Internal(FInventoryItemSlot(InInventoryItem, InventoryItem.Coordinate));
			
			return true;
		}
	}
	
	UE_LOG(LogInventory, Warning, TEXT("Can't remove inventory item, no matches"))
	
	return false;
}

bool UInventoryComponent::RemoveInventoryItemAtCoordinate(const FInventoryItemSlot& InInventoryItemSlot)
{
	if(CheckItemReference(InInventoryItemSlot.InventoryItem)) { return false; }
	
	const FInventoryItemData ItemData = InInventoryItemSlot.InventoryItem->GetInventoryItemData();

	if(!CheckInventoryItemData(ItemData)) { return false; }
	
	if(!CheckInventoryItemCoordinate(InInventoryItemSlot.Coordinate)) { return false; }
		
	for (const auto &InventoryItem : InventoryItems)
	{
		if (InventoryItem == InInventoryItemSlot)
		{
			RemoveInventoryItem_Internal(InInventoryItemSlot);
			
			return true;
		}
	}

	UE_LOG(LogInventory, Warning, TEXT("Can't remove inventory item at coordinate, no matches"))
	
	return false;
}

void UInventoryComponent::RemoveInventoryItem_Internal(const FInventoryItemSlot& InInventoryItemSlot)
{
	InventoryItems.Remove(InInventoryItemSlot);
	
	bIsSimulatedInventoryDirty = true;
	
	if (OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }

	if (OnInventoryItemRemoved.IsBound()) { OnInventoryItemRemoved.Broadcast(InInventoryItemSlot.InventoryItem); }

	UE_LOG(LogInventory, Warning, TEXT("Inventory item %s removed"), *InInventoryItemSlot.InventoryItem->GetInventoryItemData().Name.ToString())
	
}

bool UInventoryComponent::RemoveCollectibleItem(ACollectibleItem* InCollectibleItem)
{
	if (!CheckItemReference(InCollectibleItem)) { return false; }
	
	CollectibleItems.Remove(InCollectibleItem);
	
	if (OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }

	if (OnCollectibleItemRemoved.IsBound()) { OnCollectibleItemRemoved.Broadcast(InCollectibleItem); }

	UE_LOG(LogInventory, Warning, TEXT("Collectible item %s removed"), &InCollectibleItem->GetCollectibleItemData().Name)
	
	return true;
}

bool UInventoryComponent::RemoveNoteItem(ANoteItem* InNoteItem)
{
	if (!CheckItemReference(InNoteItem)) { return false; }
	
	NoteItems.Remove(InNoteItem);
	
	if (OnInventoryUpdated.IsBound()) { OnInventoryUpdated.Broadcast(); }

	if (OnNoteItemRemoved.IsBound()) { OnNoteItemRemoved.Broadcast(InNoteItem); }

	UE_LOG(LogInventory, Warning, TEXT("Note item %s removed"), &InNoteItem->GetNoteItemData().Name)
	
	return true;
}

bool UInventoryComponent::DropInventoryItem(AInventoryItem* InInventoryItem)
{
	if (!CheckItemReference(InInventoryItem)) { return false; }
	
	if (!RemoveInventoryItem(InInventoryItem)) { return false; }
	
	APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner());

	if(!OwningCharacter) { return false; }
	
	InInventoryItem->SetActorLocation(OwningCharacter->GetItemDropPosition());
	
	return true;
}

bool UInventoryComponent::DropInventoryItemFromCoordinate(const FInventoryItemSlot& InInventoryItemSlot)
{
	if (!CheckItemReference(InInventoryItemSlot.InventoryItem)) { return false; }
	
	if (!RemoveInventoryItemAtCoordinate(InInventoryItemSlot.InventoryItem)) { return false; }
	
	APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner());

	if(!OwningCharacter) { return false; }
	
	InInventoryItemSlot.InventoryItem->SetActorLocation(OwningCharacter->GetItemDropPosition());
	
	return true;
}

bool UInventoryComponent::DropCollectibleItem(ACollectibleItem* InCollectibleItem)
{
	if (!CheckItemReference(InCollectibleItem)) { return false; }
	
	if (!RemoveCollectibleItem(InCollectibleItem)) { return false; }
	
	APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner());

	if(!OwningCharacter) { return false; }
	
	InCollectibleItem->SetActorLocation(OwningCharacter->GetItemDropPosition());
	
	return true;
}

AInventoryItem* UInventoryComponent::SpawnInventoryItem(const FInventoryItemData& InInventoryItemInfo)
{
	AInventoryItem* InventoryItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(AInventoryItem::StaticClass(), FTransform());
		
	InventoryItem->SetInventoryItemData(InInventoryItemInfo);
		
	UGameplayStatics::FinishSpawningActor(InventoryItem, FTransform());

	return InventoryItem;
}

ACollectibleItem* UInventoryComponent::SpawnCollectibleItem(const FCollectibleItemData& InCollectibleItemData)
{
	ACollectibleItem* CollectibleItem = GetWorld()->SpawnActorDeferred<ACollectibleItem>(AInventoryItem::StaticClass(), FTransform());
		
	CollectibleItem->SetCollectibleItemData(InCollectibleItemData);
		
	UGameplayStatics::FinishSpawningActor(CollectibleItem, FTransform());

	return CollectibleItem;
}

ANoteItem* UInventoryComponent::SpawnNoteItem(const FNoteItemData& InNoteItemData)
{
	ANoteItem* NoteItem = GetWorld()->SpawnActorDeferred<ANoteItem>(ANoteItem::StaticClass(), FTransform());
		
	NoteItem->SetNoteItemData(InNoteItemData);
		
	UGameplayStatics::FinishSpawningActor(NoteItem, FTransform());

	return NoteItem;
}

bool UInventoryComponent::UseInventoryItem(AInventoryItem* InInventoryItem)
{
	if (!GetOwner()) { return false; }

	if (!CheckItemReference(InInventoryItem)) { return false; }
	
	APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!OwningCharacter) { return false; }
	
	InInventoryItem->UseItem(OwningCharacter);
	
	return true;
	
}



bool UInventoryComponent::HasInventoryItem(const FInventoryItemData& InInventoryItemData)
{
	for (const auto &InventoryItem : InventoryItems)
	{
		if (InventoryItem.InventoryItem->GetInventoryItemData() == InInventoryItemData)
		{
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::HasCollectibleItem(const ACollectibleItem* InCollectibleItemData)
{
	for (const auto CollectibleItem : CollectibleItems)
	{
		if (CollectibleItem == InCollectibleItemData)
		{
			return true;
		}
	}
	return false;
}

const TArray<FInventoryItemSlot>& UInventoryComponent::GetInventoryItems()
{
	return InventoryItems;
}

TArray<ACollectibleItem*> UInventoryComponent::GetCollectibleItems()
{
	return CollectibleItems;
}

TArray<ANoteItem*> UInventoryComponent::GetNoteItems()
{
	return NoteItems;
}

const FInventorySize& UInventoryComponent::GetInventorySize()
{
	return InventorySize;
}

TArray<FInventoryItemData>& UInventoryComponent::GetSimulatedInventory()
{
	if (bIsSimulatedInventoryDirty)
	{
		SimulatedInventory.Empty(InventorySize.X * InventorySize.Y);
		SimulatedInventory.Init(FInventoryItemData(),InventorySize.X * InventorySize.Y);
		
		for (auto const InventoryItem : InventoryItems)
		{
			for (int I = InventoryItem.Coordinate.X; I < InventoryItem.Coordinate.X + InventoryItem.InventoryItem->GetInventoryItemData().Size.X; ++I)
			{
				for (int J = InventoryItem.Coordinate.Y; J < InventoryItem.Coordinate.Y + InventoryItem.InventoryItem->GetInventoryItemData().Size.Y; ++J)
				{
					SimulatedInventory[CoordinateToIndex(FItemCoordinate(I, J))] = InventoryItem.InventoryItem->GetInventoryItemData();
				}
			}
		}
	}
	return SimulatedInventory;
}

bool UInventoryComponent::CanFitItemAtCoordinate(const FInventoryItemSlot& InventoryItemSlot)
{
	if (!CheckItemReference(InventoryItemSlot.InventoryItem)) { return false; }
	
	const FInventoryItemData ItemData = InventoryItemSlot.InventoryItem->GetInventoryItemData();

	if (!CheckInventoryItemData(ItemData)) { return false; }
	
	if (!CheckInventoryItemCoordinate(InventoryItemSlot.Coordinate)) { return false; }
	
	for (int I = InventoryItemSlot.Coordinate.X; I < InventoryItemSlot.Coordinate.X + InventoryItemSlot.InventoryItem->GetInventoryItemData().Size.X; ++I)
	{
		for (int J = InventoryItemSlot.Coordinate.Y; J < InventoryItemSlot.Coordinate.Y + InventoryItemSlot.InventoryItem->GetInventoryItemData().Size.Y; ++J)
		{
			const int32 Index = CoordinateToIndex(FItemCoordinate(I, J));
			if (!GetSimulatedInventory()[Index].IsEmpty())
			{
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::CanFitItemSomewhere(AInventoryItem* InInventoryItem, FItemCoordinate& OutCoordinate)
{
	for (int I = 0; I < GetSimulatedInventory().Num(); ++I)
	{
		if (GetSimulatedInventory()[I].IsEmpty())
		{
			if (CanFitItemAtCoordinate(FInventoryItemSlot(InInventoryItem, FItemCoordinate(IndexToCoordinate(I)))))
			{
				OutCoordinate = IndexToCoordinate(I);
				return true;
			}
		}
	}
	return false;
}

int32 UInventoryComponent::CoordinateToIndex(const FItemCoordinate& InCoordinate)
{
	const int32 Result = FMath::Clamp(InventorySize.X * InCoordinate.X + InCoordinate.Y,
					0,
					InventorySize.X * InventorySize.Y - 1);
	return Result;
}

FItemCoordinate UInventoryComponent::IndexToCoordinate(const int32& InIndex)
{
	int32 const Row = FMath::RoundToNegativeInfinity(static_cast<float>(InIndex) / InventorySize.X);
	int32 const Column = InIndex % InventorySize.X; 
	return FItemCoordinate(FMath::Clamp(Row, 0, InventorySize.X), FMath::Clamp(Column, 0, InventorySize.Y));
}

bool UInventoryComponent::CheckItemReference(const AItem* InItem)
{
	if (!InItem) { UE_LOG(LogInventory, Error, TEXT("Can't add item, ref is null")) return false; }
	return true;
}

bool UInventoryComponent::CheckInventoryItemData(const FInventoryItemData& InInventoryItemData)
{
	if (InInventoryItemData.IsEmpty()) { UE_LOG(LogInventory, Error, TEXT("Can't add inventory item, data is empty")) return false; }
	return true;
}

bool UInventoryComponent::CheckInventoryItemSize(const FInventoryItemData& InInventoryItemData) const
{
	if (InInventoryItemData.Size.X <= 0 || InInventoryItemData.Size.Y <= 0)
	{
		UE_LOG(LogInventory, Error, TEXT("Can't add inventory item, item size is contains zero in one or both dimensions"))
	}
	
	if (InInventoryItemData.Size.X > InventorySize.X || InInventoryItemData.Size.Y > InventorySize.Y)
	{
		UE_LOG(LogInventory, Error, TEXT("Can't add inventory item, item size is [%d, %d], inventory size is [%d, %d]"),
		InInventoryItemData.Size.X ,InInventoryItemData.Size.Y, InventorySize.X, InventorySize.Y) return false;
	}

	
	
	return true;
}

bool UInventoryComponent::CheckInventoryItemCoordinate(const FItemCoordinate& InItemCoordinate) const
{
	if (InItemCoordinate.X < 0 || InItemCoordinate.X > InventorySize.X
		|| InItemCoordinate.Y < 0 || InItemCoordinate.Y > InventorySize.Y)
	{ UE_LOG(LogInventory, Error, TEXT("Can't add inventory item, coordinate [%d, %d] is not valid"), InItemCoordinate.X, InItemCoordinate.Y) return false; }
	return true;
}






