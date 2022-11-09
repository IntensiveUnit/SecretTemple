// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/CollectibleItem.h"
#include "Objects/InventoryItem.h"

//TODO custom log
UInventoryComponent::UInventoryComponent():
bIsSimulatedInventoryDirty(true),
InventorySize(7, 7),
CellSize(50.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	
	for (const auto& DataTableRowHandle : StartupItems)
	{
		if (!DataTableRowHandle.DataTable || DataTableRowHandle.RowName.IsNone()) { continue; }
		
		FItemInformation ItemInfo = DataTableRowHandle.DataTable->FindRow<FItemInformation>(DataTableRowHandle.RowName, "");
		
		if (ItemInfo.IsEmpty()){ continue; }
		
		AInventoryItem* InventoryItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(AInventoryItem::StaticClass(), FTransform());
		InventoryItem->SetItemInfo(ItemInfo);
		InventoryItem->HideItem();
		UGameplayStatics::FinishSpawningActor(InventoryItem, FTransform());
		AddItem(FInventoryItemInfo(InventoryItem, FItemCoordinate(0,0)));
	}

	
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto& CollectibleItem : StartupCollectibleItems)
    	{
    		if (!CollectibleItem) { continue; }
    		
    		AddCollectibleItem(CollectibleItem);
    	}
}

bool UInventoryComponent::AddItem(const FInventoryItemInfo& InInventoryItemInfo /* Default coordinate {0, 0} */)
{
	if (!InInventoryItemInfo.Item) { UE_LOG(LogTemp, Warning, TEXT("Can't add item, ref is empty")) return false; }
	//TODO add onscreen debug messages if error fatal
	if (InInventoryItemInfo.Item->GetItemInfo().IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't add item, item info is empty")) return false; }

	if (InInventoryItemInfo.Item->GetItemInfo().Size.X > InventorySize.X || InInventoryItemInfo.Item->GetItemInfo().Size.Y > InventorySize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, item size is [%d, %d], inventory size is [%d, %d]"),
		InInventoryItemInfo.Item->GetItemInfo().Size.X ,InInventoryItemInfo.Item->GetItemInfo().Size.Y, InventorySize.X, InventorySize.Y) return false;
	}

	if (InInventoryItemInfo.Coordinate.X < 0 || InInventoryItemInfo.Coordinate.X > InventorySize.X || InInventoryItemInfo.Coordinate.Y < 0 || InInventoryItemInfo.Coordinate.Y > InventorySize.Y)
	{ UE_LOG(LogTemp, Warning, TEXT("Can't add item, coordinate [%d, %d] is not valid"), InInventoryItemInfo.Coordinate.X, InInventoryItemInfo.Coordinate.Y) return false; }
	
	if (CanFitItemAtCoordinate(InInventoryItemInfo.Item->GetItemInfo(), InInventoryItemInfo.Coordinate))
	{
		AddItem_Internal(InInventoryItemInfo);
	
		OnInventoryUpdated.Broadcast();
	
		return true;
	}
	
	FItemCoordinate Coordinate;
	if (CanFitItemSomewhere(InInventoryItemInfo.Item->GetItemInfo(),Coordinate))
	{
		AddItem_Internal(FInventoryItemInfo(InInventoryItemInfo.Item, Coordinate));
		
		OnInventoryUpdated.Broadcast();

		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Can't add item, no space"))
	return false;
}

bool UInventoryComponent::AddCollectibleItem(ACollectibleItem* InCollectibleItem)
{
	CollectibleItems.Add(InCollectibleItem);
	return true;
}


void UInventoryComponent::AddItem_Internal(const FInventoryItemInfo& InInventoryItemInfo)
{
	InventoryItems.Add(InInventoryItemInfo);
	bIsSimulatedInventoryDirty = true;
}

bool UInventoryComponent::RemoveItem(const FInventoryItemInfo& InInventoryItemInfo)
{
	//TODO there chains of structure->reference->Function->AnotherFunction is so bad, need strong refactoring
	if (InInventoryItemInfo.Item->GetItemInfo().IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't remove item, item is empty")) return false; }

	if (InInventoryItemInfo.Coordinate.X < 0 || InInventoryItemInfo.Coordinate.X > InventorySize.X ||
		InInventoryItemInfo.Coordinate.Y < 0 || InInventoryItemInfo.Coordinate.Y > InventorySize.Y)
	{ UE_LOG(LogTemp, Warning, TEXT("Can't remove item, coordinates not valid")) return false; }

	//Find equal by info and coordinate
	for (const auto &Item : InventoryItems)
	{
		if (Item == InInventoryItemInfo)
		{
			RemoveItem_Internal(InInventoryItemInfo);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	//Find equal only by info
	for (const auto &Item : InventoryItems)
	{
		if (Item.Item->GetItemInfo() == InInventoryItemInfo.Item->GetItemInfo())
		{
			RemoveItem_Internal(FInventoryItemInfo(Item.Item, Item.Coordinate));
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	
	return false;
}

void UInventoryComponent::RemoveItem_Internal(const FInventoryItemInfo& InInventoryItemInfo)
{
	InventoryItems.Remove(InInventoryItemInfo);
	bIsSimulatedInventoryDirty = true;
}

bool UInventoryComponent::RemoveCollectibleItem(ACollectibleItem* InCollectibleItem)
{
	if (!InCollectibleItem){ return false; }
	if (!CollectibleItems.Contains(InCollectibleItem)) { return false; }
	CollectibleItems.Remove(InCollectibleItem);
	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventoryComponent::UseItem(const FInventoryItemInfo& InInventoryItemInfo)
{
	if(!GetOwner()) { return false; }
	APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner());

	//Find equal by info and coordinate
	for (auto const &Item : InventoryItems)
	{
		if (Item == InInventoryItemInfo)
		{
			Item.Item->UseItem(OwningCharacter);
			RemoveItem_Internal(InInventoryItemInfo);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	//Find equal only by info
	for (auto const &Item : InventoryItems)
	{
		if (Item.Item->GetItemInfo() == InInventoryItemInfo.Item->GetItemInfo())
		{
			Item.Item->UseItem(OwningCharacter);
			RemoveItem_Internal(InInventoryItemInfo);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	return false;
}



int32 UInventoryComponent::CoordinateToIndex(const FItemCoordinate& InCoordinate)
{
	const int32 Result = FMath::Clamp(InventorySize.X * InCoordinate.X + InCoordinate.Y,
					0,
					SimulatedInventory.Num() - 1);
	return Result;
}

FItemCoordinate UInventoryComponent::IndexToCoordinate(const int32& InIndex)
{
	int32 const Row = FMath::RoundToNegativeInfinity(static_cast<float>(InIndex) / InventorySize.X);
	int32 const Column = InIndex % InventorySize.X; 
	return FItemCoordinate(FMath::Clamp(Row, 0, InventorySize.X), FMath::Clamp(Column, 0, InventorySize.Y));
}


bool UInventoryComponent::CanFitItemAtCoordinate(const FItemInformation& InItemInfo, const FItemCoordinate& InCoordinate)
{
	if (InCoordinate.X + InItemInfo.Size.X > InventorySize.X || InCoordinate.Y + InItemInfo.Size.Y > InventorySize.Y)
	{ return false; }

	const TArray<FItemInformation>& SimulatedInventoryCache = GetSimulatedInventory();
	
	for (int I = InCoordinate.X; I < InCoordinate.X + InItemInfo.Size.X; ++I)
	{
		for (int J = InCoordinate.Y; J < InCoordinate.Y + InItemInfo.Size.Y; ++J)
		{
			const int32 Index = CoordinateToIndex(FItemCoordinate(I, J));
			if (!SimulatedInventoryCache[Index].IsEmpty())
			{
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::CanFitItemSomewhere(const FItemInformation& InItemInfo, FItemCoordinate& OutCoordinate)
{
	for (int I = 0; I < GetSimulatedInventory().Num(); ++I)
	{
		if (GetSimulatedInventory()[I].IsEmpty())
		{
			if (CanFitItemAtCoordinate(InItemInfo, FItemCoordinate(IndexToCoordinate(I))))
			{
				OutCoordinate = IndexToCoordinate(I);
				return true;
			}
		}
	}
	return false;
}

TArray<FItemInformation>& UInventoryComponent::GetSimulatedInventory()
{
	if (bIsSimulatedInventoryDirty)
	{
		SimulatedInventory.Empty(InventorySize.X * InventorySize.Y);
		SimulatedInventory.Init(FItemInformation(),InventorySize.X * InventorySize.Y);
		
		for (auto const InventoryItem : InventoryItems)
		{
			for (int I = InventoryItem.Coordinate.X; I < InventoryItem.Coordinate.X + InventoryItem.Item->GetItemInfo().Size.X; ++I)
			{
				for (int J = InventoryItem.Coordinate.Y; J < InventoryItem.Coordinate.Y + InventoryItem.Item->GetItemInfo().Size.Y; ++J)
				{
					SimulatedInventory[CoordinateToIndex(FItemCoordinate(I, J))] = InventoryItem.Item->GetItemInfo();
				}
			}
		}
		bIsSimulatedInventoryDirty = false;
	}

	return SimulatedInventory;
}

bool UInventoryComponent::HasItem(const FItemInformation& InItemInfo)
{
	for (const auto InventoryItem : InventoryItems)
	{
		if (InventoryItem.Item->GetItemInfo() == InItemInfo)
		{
			return true;
		}
	}
	return false;
}

TArray<FInventoryItemInfo>& UInventoryComponent::GetItems()
{
	return InventoryItems;
}

TArray<ACollectibleItem*> UInventoryComponent::GetCollectibleItems()
{
	return CollectibleItems;
}


//Need delete this or rewriting
TArray<FItemCoordinate> UInventoryComponent::GetAllCells()
{
	TArray<FItemCoordinate> AllCells;

	for (int I = 0; I < InventorySize.X; ++I)
	{
		for (int J = 0; J < InventorySize.Y; ++J)
		{
			AllCells.Add(FItemCoordinate(I, J));
		}
	}

	return AllCells;
}


FInventorySize UInventoryComponent::GetInventorySize()
{
	return InventorySize;
}




