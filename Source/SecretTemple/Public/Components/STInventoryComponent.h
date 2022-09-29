// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/STItem.h"
#include "STInventoryComponent.generated.h"

/**
 * Delegates
 */

class ACollectibleItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemAdded, struct FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryCollectibleItemAdded, class ACollectibleItem*, Item);

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItem Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Coordinate;

	FInventoryItem():
	Item(FItem()),
	Coordinate(FIntPoint())
	{}
	
	FInventoryItem(const FItem& InItem, const FIntPoint& InCoordinate):
	Item(InItem),
	Coordinate(InCoordinate)
	{}
	
	bool operator ==(const FInventoryItem* InItem) const
	{
		return this->Item == InItem->Item && this->Coordinate == InItem->Coordinate;
	}

	bool operator ==(const FInventoryItem InItem) const
	{
		return this->Item == InItem.Item && this->Coordinate == InItem.Coordinate;
	}
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETTEMPLE_API USTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//Functions, etc...
	USTInventoryComponent();

	virtual void OnComponentCreated() override;
	
	//Properties
	
	TArray<FItem> SimulatedInventory;

public:
	//Functions, etc...
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FItem& InItem);

	UFUNCTION(BlueprintCallable)
	bool AddCollectibleItemToInventory(ACollectibleItem* InCollectibleItem);

	UFUNCTION(BlueprintCallable)
	bool AddItemAtCoordinates(const FItem& InItem, const FIntPoint& InCoordinate);
	
	void AddItem_Internal(const FItem& InItem, const FIntPoint& InCoordinate);

	UFUNCTION(BlueprintCallable)
	bool CanFitItemAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate);
	
	UFUNCTION(BlueprintCallable)
	bool CanFitItemSomewhere(const FItem& InItem, FIntPoint& OutCoordinate);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(const FItem& InItem);

	UFUNCTION(BlueprintCallable)
	bool RemoveCollectibleItemFromInventory(ACollectibleItem* InCollectibleItem);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventoryAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate);

	void RemoveItemFromInventory_Internal(const FItem& InItem, const FIntPoint& InCoordinate);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasItem (const FItem& Item);

	UFUNCTION(BlueprintCallable)
	bool UseItem(const FItem& InItem);

	UFUNCTION(BlueprintCallable)
	bool UseItemAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate );

	UFUNCTION(BlueprintCallable)
	ASTItem* DropItemInWorld(const FItem& InItem);
	//TODO name in this case is not good
	UFUNCTION(BlueprintCallable)
	ASTItem* DropItemInWorldAtCoordinate(const FItem& InItem, const FIntPoint& InCoordinate);

	ASTItem* DropItemInWorld_Internal(const FItem& InItem);
	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FInventoryItem>& GetItems();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ACollectibleItem*> GetCollectibleItems();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FIntPoint> GetAllCells();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint GetInventorySize();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CoordinateToIndex(const FIntPoint& InCoordinate);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint IndexToCoordinate(const int32& InIndex);
	
	//Properties
	UPROPERTY()
	TArray<FInventoryItem> Items;

	UPROPERTY()
	TArray<ACollectibleItem*> CollectibleItems;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint InventorySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1.0f, UIMin = 1.0f), Category = "Inventory")
	float CellSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDataTableRowHandle> StartupStandardItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ACollectibleItem*> StartupCollectibleItems;

	//Delegates
	UPROPERTY(BlueprintAssignable)	
	FInventoryEvent OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)	
	FInventoryItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)	
	FInventoryCollectibleItemAdded OnCollectibleItemAdded;
	
};


