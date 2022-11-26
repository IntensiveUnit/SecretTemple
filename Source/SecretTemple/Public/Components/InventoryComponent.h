// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/CollectibleItem.h"
#include "Objects/InventoryItem.h"
#include "Objects/NoteItem.h"
#include "InventoryComponent.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FInventorySize
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "2", UIMin = "2"))
	int32 X = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "2", UIMin = "2"))
	int32 Y = 2;

	FInventorySize():
	X(2),
	Y(2)
	{}

	FInventorySize(const int32 InX, const int32 InY):
	X(InX >= 2 ? InX : 2),
	Y(InY >= 2 ? InY : 2)
	{}

	FInventorySize(const FInventorySize& InInventorySize):
	X(InInventorySize.X),
	Y(InInventorySize.Y)
	{}

	bool operator ==(const FInventorySize& InInventorySize) const
	{
		return this->X == InInventorySize.X && this->Y == InInventorySize.Y;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemCoordinate
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y = 0;

	FItemCoordinate():
	X(0),
	Y(0)
	{}

	FItemCoordinate(const int32 InX, const int32 InY):
	X(InX),
	Y(InY)
	{}

	FItemCoordinate(const FItemCoordinate& InItemCoordinate):
	X(InItemCoordinate.X),
	Y(InItemCoordinate.Y)
	{}

	bool operator ==(const FItemCoordinate& InItemCoordinate) const
	{
		return this->X == InItemCoordinate.X && this->Y == InItemCoordinate.Y;
	}
};


USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItemSlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AInventoryItem* InventoryItem = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FItemCoordinate Coordinate = {0,0};

	FInventoryItemSlot():
		InventoryItem(nullptr),
		Coordinate(0,0)
	{}

	FInventoryItemSlot(AInventoryItem* InInventoryItem):
		InventoryItem(InInventoryItem),
		Coordinate(0,0)
	{}

	FInventoryItemSlot(AInventoryItem* InInventoryItem, const FItemCoordinate& InCoordinate):
		InventoryItem(InInventoryItem),
		Coordinate(InCoordinate)
	{}
	
	bool operator ==(const FInventoryItemSlot& InItem) const
	{
		return this->InventoryItem == InItem.InventoryItem && this->Coordinate == InItem.Coordinate;
	}
	
};

/**
 * Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemAdded, const AInventoryItem*, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectibleItemAdded, const ACollectibleItem*, CollectibleItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoteItemAdded, const ANoteItem*, NoteItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemRemoved, const AInventoryItem*, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectibleItemRemoved, const ACollectibleItem*, CollectibleItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoteItemRemoved, const ANoteItem*, NoteItem);

class ACollectibleItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETTEMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	
	//------------------------------------------------------------------//
	//----------------------Internal------------------------------------//
	//------------------------------------------------------------------//

	//Constructor is used to initialize variables
	UInventoryComponent();

	//When the component is created we can add starting items to the inventory
	virtual void OnComponentCreated() override;
	
	UPROPERTY()
	TArray<FInventoryItemSlot> InventoryItems;
	
	UPROPERTY()
	TArray<ACollectibleItem*> CollectibleItems;

	UPROPERTY()
	TArray<ANoteItem*> NoteItems;

	//Inventory simulation involves creating a two-dimensional array and filling each cell
	//with information depending on the coordinates of the available items,
	//so as not to do these calculations every time a new item is added,
	//i use this array and DirtyFlag
	UPROPERTY(Transient)
	TArray<FInventoryItemData> SimulatedInventory;
	
	UPROPERTY(Transient)
	bool bIsSimulatedInventoryDirty;

public:

	//----------------------------------------------------------------//
	//----------------------Properties--------------------------------//
	//----------------------------------------------------------------//

	/**
	* Inventory size in two dimensions
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FInventorySize InventorySize;
	
	/**
	* Inventory items that give at the start
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FDataTableRowHandle> StartingInventoryItems;
	
	/**
	* Collectible items that give at the start
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FDataTableRowHandle> StartingCollectibleItems;

	/**
	* Note items that give at the start
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FDataTableRowHandle> StartingNoteItems;
	
	//----------------------------------------------------------------//
	//----------------------Adding items------------------------------//
	//----------------------------------------------------------------//
	
	/**
	* Add inventory item to inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool AddInventoryItem(AInventoryItem* InInventoryItem);
	
	/**
	* Add an inventory item to inventory at a specific coordinate
	*/
	UFUNCTION(BlueprintCallable)
	bool AddInventoryItemAtCoordinate(const FInventoryItemSlot& InInventoryItemSlot);

	//Internal function for adding inventory items
	void AddInventoryItem_Internal(const FInventoryItemSlot& InInventoryItemSlot);
	
	/**
	* Add a collectible item
	*/
	UFUNCTION(BlueprintCallable)
	bool AddCollectibleItem(ACollectibleItem* InCollectibleItem);

	/**
	* Add a collectible item
	*/
	UFUNCTION(BlueprintCallable)
	bool AddNoteItem(ANoteItem* InNoteItem);
	
	//----------------------------------------------------------------//
	//----------------------Removing items----------------------------//
	//----------------------------------------------------------------//
	
	/**
	* Remove any similar inventory item from inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveInventoryItem(AInventoryItem* InInventoryItem);

	/**
	* Remove inventory item from inventory at coordinate
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveInventoryItemAtCoordinate(const FInventoryItemSlot& InInventoryItemSlot);
	
	//Internal function for removing inventory items
	void RemoveInventoryItem_Internal(const FInventoryItemSlot& InInventoryItemSlot);

	/**
	* Remove  collectible item
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveCollectibleItem(ACollectibleItem* InCollectibleItem);

	/**
	* Remove note item
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveNoteItem(ANoteItem* InNoteItem);

	//------------------------------------------------------------------//
	//----------------------Dropping items------------------------------//
	//------------------------------------------------------------------//
	
	/**
	* Drop any similar inventory item from inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool DropInventoryItem(AInventoryItem* InInventoryItem);
	
	/**
	* Drop an inventory item at specific coordinate into the world
	*/
	UFUNCTION(BlueprintCallable)
	bool DropInventoryItemFromCoordinate(const FInventoryItemSlot& InInventoryItemSlot);
	
	/**
	* Drop any similar collectible item
	*/
	UFUNCTION(BlueprintCallable)
	bool DropCollectibleItem(ACollectibleItem* InCollectibleItem);

	//------------------------------------------------------------------//
	//----------------------Spawning items------------------------------//
	//------------------------------------------------------------------//

	/**
	* Spawn inventory item 
	*/
	UFUNCTION(BlueprintCallable)
	AInventoryItem* SpawnInventoryItem(const FInventoryItemData& InInventoryItemInfo);

	/**
	* Spawn collectible item
	*/
	UFUNCTION(BlueprintCallable)
	ACollectibleItem* SpawnCollectibleItem(const FCollectibleItemData& InCollectibleItemData);

	/**
	* Spawn note item
	*/
	UFUNCTION(BlueprintCallable)
	ANoteItem* SpawnNoteItem(const FNoteItemData& InNoteItemData);


	
	//----------------------------------------------------------------//
	//----------------------Using items-------------------------------//
	//----------------------------------------------------------------//

	/**
	* Use inventory item, means apply UGameplayEffect
	*/
	UFUNCTION(BlueprintCallable)
	bool UseInventoryItem(AInventoryItem* InInventoryItem);

	
	
	//------------------------------------------------------------------//
	//----------------------Helpers-------------------------------------//
	//------------------------------------------------------------------//

	/**
	* Has a similar inventory item in inventory
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasInventoryItem (const FInventoryItemData& InInventoryItemData);

	/**
	* Has a similar collectible item in inventory
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasCollectibleItem (const ACollectibleItem* InCollectibleItemData);

	/**
	* Get all inventory items
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TArray<FInventoryItemSlot>& GetInventoryItems();

	/**
	* Get all collectible items
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ACollectibleItem*> GetCollectibleItems();

	/**
	* Get all note items
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ANoteItem*> GetNoteItems();

	/**
	* Take two dimensional inventory size
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FInventorySize& GetInventorySize();
	
	/**
	* Take simulated representation of two-dimensional inventory, specified as a one-dimensional array
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FInventoryItemData>& GetSimulatedInventory();
	
	/**
	* Is it possible to fit an inventory item of the specified size by coordinate
	*/
	UFUNCTION(BlueprintCallable)
	bool CanFitItemAtCoordinate(const FInventoryItemSlot& InventoryItemSlot);
	
	/**
	* Is it possible to fit an inventory item of the specified size somewhere in the inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool CanFitItemSomewhere(AInventoryItem* InInventoryItem, FItemCoordinate& OutCoordinate);

	/**
	* Represent two-dimensional coordinates as an index for a one-dimensional array
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CoordinateToIndex(const FItemCoordinate& InCoordinate);

	/**
	* Represent one-dimensional array index as two-dimensional coordinates
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemCoordinate IndexToCoordinate(const int32& InIndex);

	/**
	* Checking and logging
	*/
	static bool CheckItemReference(const AItem* InItem);

	static bool CheckInventoryItemData(const FInventoryItemData& InInventoryItemData);
	
	bool CheckInventoryItemSize(const FInventoryItemData& InInventoryItemData) const;

	bool CheckInventoryItemCoordinate(const FItemCoordinate& InItemCoordinate) const;
	

	
	//------------------------------------------------------------------//
	//----------------------Delegates-----------------------------------//
	//------------------------------------------------------------------//
	UPROPERTY(BlueprintAssignable)	
	FInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)	
	FInventoryItemAdded OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable)	
	FCollectibleItemAdded OnCollectibleItemAdded;

	UPROPERTY(BlueprintAssignable)	
	FNoteItemAdded OnNoteItemAdded;

	UPROPERTY(BlueprintAssignable)	
	FInventoryItemRemoved OnInventoryItemRemoved;

	UPROPERTY(BlueprintAssignable)	
	FCollectibleItemRemoved OnCollectibleItemRemoved;

	UPROPERTY(BlueprintAssignable)	
	FNoteItemRemoved OnNoteItemRemoved;
};







