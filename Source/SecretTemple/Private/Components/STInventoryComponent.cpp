// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STInventoryComponent.h"

#include "Character/STCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/CollectibleItem.h"


// Sets default values for this component's properties
USTInventoryComponent::USTInventoryComponent():
InventorySize(7, 7),
CellSize(50.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}

void USTInventoryComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SimulatedInventory.Empty(InventorySize.X * InventorySize.Y);
	SimulatedInventory.Init(FItem(),InventorySize.X * InventorySize.Y);
	
	for (const auto& DataTableRowHandle : StartupStandardItems)
	{
		if (!DataTableRowHandle.DataTable || DataTableRowHandle.RowName.IsNone()) { continue; }
		
		FItem ItemInfo = DataTableRowHandle.DataTable->FindRow<FItem>(DataTableRowHandle.RowName, "");
		
		if (!ItemInfo.IsEmpty()){ continue; }
		
		AddItem(ItemInfo);
	}

	for (const auto& CollectibleItem : StartupCollectibleItems)
	{
		if (!CollectibleItem) { continue; }
		
		AddCollectibleItemToInventory(CollectibleItem);
	}
	
}


int32 USTInventoryComponent::CoordinateToIndex(const FIntPoint& InCoordinate)
{
	return FMath::Clamp(InventorySize.X * InCoordinate.X + InCoordinate.Y,
					0,
					SimulatedInventory.Num() - 1);
}

FIntPoint USTInventoryComponent::IndexToCoordinate(const int32& InIndex)
{
	int32 const Row = FMath::RoundToNegativeInfinity(static_cast<float>(InIndex) / static_cast<float>(InventorySize.X));
	int32 const Column = InIndex % InventorySize.Y;
	return FIntPoint(FMath::Clamp(Row, 0, InventorySize.X), FMath::Clamp(Column, 0, InventorySize.Y));
}

bool USTInventoryComponent::AddItem(const FItem& InItem)
{
	if (InItem.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't add item, item is empty")) return false; }

	if (InItem.Size.X > InventorySize.X || InItem.Size.Y > InventorySize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, item size is [%d %d], inventody size id [%d %d]"),
		InItem.Size.X ,InItem.Size.Y, InventorySize.X, InventorySize.Y) return false;
	}

	FIntPoint Coordinate;
	const bool CanAddItem = CanFitItemSomewhere(InItem,Coordinate);
	
	if (CanAddItem)
	{
		AddItem_Internal(InItem, Coordinate);
		
		OnInventoryUpdated.Broadcast();
	}
	
	return CanAddItem;
	
}

bool USTInventoryComponent::AddCollectibleItemToInventory(ACollectibleItem* InCollectibleItem)
{
	CollectibleItems.Add(InCollectibleItem);
	return true;
}

bool USTInventoryComponent::AddItemAtCoordinates(const FItem& InItem, const FIntPoint& InCoordinate)
{
	if (InItem.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't add item, item is empty")) return false; }

	if (InItem.Size.X > InventorySize.X || InItem.Size.Y > InventorySize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, item size is [%d %d], inventody size id [%d %d]"),
		InItem.Size.X ,InItem.Size.Y, InventorySize.X, InventorySize.Y) return false;
	}

	if (InCoordinate.X < 0 || InCoordinate.X > InventorySize.X ||
		InCoordinate.Y < 0 || InCoordinate.Y > InventorySize.Y)
	{ UE_LOG(LogTemp, Warning, TEXT("Can't add item, coordinates not valid")) return false; }
	
	if (!CanFitItemAtCoordinate(InItem, InCoordinate)){ UE_LOG(LogTemp, Warning, TEXT("Can't add item, no space")) return false; }
	
	AddItem_Internal(InItem, InCoordinate);
	
	OnInventoryUpdated.Broadcast();
	
	return true;
}

void USTInventoryComponent::AddItem_Internal(const FItem& InItem, const FIntPoint& InCoordinate)
{
	Items.Add(FInventoryItem(InItem, InCoordinate));
	for (int I = InCoordinate.X; I < InCoordinate.X + InItem.Size.X; ++I)
	{
		for (int J = InCoordinate.Y; J < InCoordinate.Y + InItem.Size.Y; ++J)
		{
			SimulatedInventory[CoordinateToIndex(FIntPoint(I, J))] = InItem;
		}
	}
}

bool USTInventoryComponent::RemoveItemFromInventory(const FItem& InItem)
{
	if (InItem.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't remove item, item is empty")) return false; }

	FIntPoint Coordinate;
	bool CanRemoveItem = false;
	
	for (auto &Item : Items)
	{
		if (Item.Item == InItem)
		{
			CanRemoveItem = true;
			Coordinate = Item.Coordinate;
			break;
		}
	}

	if (CanRemoveItem)
	{
		RemoveItemFromInventory_Internal(InItem, Coordinate);
		
		OnInventoryUpdated.Broadcast();
	}
	
	return CanRemoveItem;
}

bool USTInventoryComponent::RemoveCollectibleItemFromInventory(ACollectibleItem* InCollectibleItem)
{
	CollectibleItems.Remove(InCollectibleItem);
	return true;
}

bool USTInventoryComponent::RemoveItemFromInventoryAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate)
{
	if (InItem.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Can't remove item, item is empty")) return false; }

	if (InCoordinate.X < 0 || InCoordinate.X > InventorySize.X ||
		InCoordinate.Y < 0 || InCoordinate.Y > InventorySize.Y)
	{ UE_LOG(LogTemp, Warning, TEXT("Can't remove item, coordinates not valid")) return false; }

	
	bool CanRemoveItem = false;
	
	for (auto &Item : Items)
	{
		if (Item.Coordinate == InCoordinate && Item.Item == InItem)
		{
			CanRemoveItem = true;
			break;
		}
	}

	if (CanRemoveItem)
	{
		RemoveItemFromInventory_Internal(InItem, InCoordinate);
		
		OnInventoryUpdated.Broadcast();
	}
	
	return CanRemoveItem;
	
}

void USTInventoryComponent::RemoveItemFromInventory_Internal(const FItem& InItem, const FIntPoint& InCoordinate)
{
	Items.Remove(FInventoryItem(InItem, InCoordinate));
	for (int I = InCoordinate.X; I < InCoordinate.X + InItem.Size.X; ++I)
	{
		for (int J = InCoordinate.Y; J < InCoordinate.Y + InItem.Size.Y; ++J)
		{
			SimulatedInventory[CoordinateToIndex(FIntPoint(I, J))] = FItem();
		}
	}
}

bool USTInventoryComponent::CanFitItemAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate)
{
	if (InCoordinate.X + InItem.Size.X > InventorySize.X ||
		InCoordinate.Y + InItem.Size.Y > InventorySize.Y)
	{
		return false;
	}
	
	for (int I = InCoordinate.X; I < InCoordinate.X + InItem.Size.X; ++I)
	{
		for (int J = InCoordinate.Y; J < InCoordinate.Y + InItem.Size.Y; ++J)
		{
			if (!SimulatedInventory[CoordinateToIndex(FIntPoint(I, J))].IsEmpty())
			{
				return false;
			}
		}
	}
	return true;
}

bool USTInventoryComponent::CanFitItemSomewhere(const FItem& InItem, FIntPoint& OutCoordinate)
{
	for (int I = 0; I < SimulatedInventory.Num(); ++I)
	{
		if (SimulatedInventory[I].IsEmpty())
		{
			if (CanFitItemAtCoordinate(InItem, FIntPoint(IndexToCoordinate(I))))
			{
				OutCoordinate = IndexToCoordinate(I);
				return true;
			}
			
		}
	}
	return false;
}

bool USTInventoryComponent::HasItem(const FItem& InItem)
{
	for (auto InventoryItem : Items)
	{
		if (InventoryItem.Item == InItem)
		{
			return true;
			
		}
	}
	return false;
}

bool USTInventoryComponent::UseItem(const FItem& InItem)
{
	FIntPoint Coordinate;
	
	bool bItemWasFounded = false;
	
	for (auto const &Item : Items)
	{
		if (Item.Item == InItem)
		{
			Coordinate = Item.Coordinate;
			bItemWasFounded = true;
		}
		break;
	}
	
	if (bItemWasFounded)
	{
		if(!GetOwner()) { return false; }
		const ASTCharacter* OwningCharacter = Cast<ASTCharacter>(GetOwner());
		
		if (!OwningCharacter) {return false; }
		ASTItem* Item = DropItemInWorldAtCoordinate(InItem, Coordinate);

		if (Item)
		{
			Item->UseItem(OwningCharacter);
		}
		
		OnInventoryUpdated.Broadcast();
	}
	
	return bItemWasFounded;
	
}


bool USTInventoryComponent::UseItemAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate)
{
	
	bool bItemWasFounded = false;
	
	for (auto const &Item : Items)
	{
		if (Item.Item == InItem && Item.Coordinate == InCoordinate)
		{
			bItemWasFounded = true;
			break;
		}
		
	}

	if (bItemWasFounded)
	{
		if(!GetOwner()) { return false; }
		const ASTCharacter* OwningCharacter = Cast<ASTCharacter>(GetOwner());
		
		if (!OwningCharacter) {return false; }
		ASTItem* Item = DropItemInWorldAtCoordinate(InItem, InCoordinate);

		if (Item)
		{
			Item->UseItem(OwningCharacter);
		}
		
		OnInventoryUpdated.Broadcast();
	}

	return bItemWasFounded;
}

ASTItem* USTInventoryComponent::DropItemInWorld(const FItem& InItem)
{
	FIntPoint Coordinate;
	
	bool bItemWasDropped = false;
	
	for (auto const &Item : Items)
	{
		if (Item.Item == InItem)
		{
			Coordinate = Item.Coordinate;
			bItemWasDropped = true;
			break;
		}
		
	}

	ASTItem* DroppedItem = nullptr;
	if (bItemWasDropped)
	{
		DroppedItem = DropItemInWorld_Internal(InItem);
		if (DroppedItem)
		{
			RemoveItemFromInventory_Internal(InItem, Coordinate);
		}
		
		OnInventoryUpdated.Broadcast();
	}
	
	return DroppedItem;
}

ASTItem* USTInventoryComponent::DropItemInWorldAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate)
{
	bool bItemWasDropped = false;

	//TODO rewrite all component with predicate finding
	for (auto const &Item : Items)
	{
		if (Item.Item == InItem && Item.Coordinate == InCoordinate)
		{
			bItemWasDropped = true;
			break;
		}
		
	}
	
	ASTItem* DroppedItem = nullptr;
	if (bItemWasDropped)
	{
		DroppedItem = DropItemInWorld_Internal(InItem);
		if (DroppedItem)
		{
			RemoveItemFromInventory_Internal(InItem, InCoordinate);
		}
		
		OnInventoryUpdated.Broadcast();
	}

	return DroppedItem;
}

ASTItem* USTInventoryComponent::DropItemInWorld_Internal(const FItem& InItem)
{
	ASTItem* DroppedItem = nullptr;
	
	const AActor* Owner = GetOwner();
	
	if (!Owner) { return DroppedItem; }

	const UWorld* World = GetWorld();
	if (!World) { return DroppedItem; }

	//TODO configure spawn parameters via owner
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 200.f;
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
	
	ASTItem* Item = GetWorld()->SpawnActorDeferred<ASTItem>(ASTItem::StaticClass(), SpawnTransform);
	Item->SetItemInfo(InItem);
	UGameplayStatics::FinishSpawningActor(Item, SpawnTransform);
	
	return Item;
}

TArray<FInventoryItem>& USTInventoryComponent::GetItems()
{
	return Items;
}

//TODO TArray<ACollectibleItem*>* Array of ref or ref on array with refs?
TArray<ACollectibleItem*> USTInventoryComponent::GetCollectibleItems()
{
	return CollectibleItems;
}


//Need delete this or rewriting
TArray<FIntPoint> USTInventoryComponent::GetAllCells()
{
	TArray<FIntPoint> AllCells;

	for (int I = 0; I < InventorySize.X; ++I)
	{
		for (int J = 0; J < InventorySize.Y; ++J)
		{
			AllCells.Add(FIntPoint(I, J));
		}
	}

	return AllCells;
}


FIntPoint USTInventoryComponent::GetInventorySize()
{
	return InventorySize;
}




