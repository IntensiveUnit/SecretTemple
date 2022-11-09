// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/Item.h"
#include "InventoryComponent.generated.h"

//This dont work with UFUNCTION
typedef FIntPoint InventorySize_t;

USTRUCT(BlueprintType, Blueprintable)
struct FItemCoordinate
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

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
struct FInventorySize
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "2", UIMin = "2"))
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2", UIMin = "2"))
	int32 Y;

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
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemCoordinate Coordinate;

	FInventoryItemInfo():
		Item(nullptr),
		Coordinate(0,0)
	{}

	FInventoryItemInfo(AItem* InItem):
		Item(InItem),
		Coordinate(0,0)
	{}

	FInventoryItemInfo(AItem* InItem, const FItemCoordinate& InCoordinate):
		Item(InItem),
		Coordinate(InCoordinate)
	{}
	
	bool operator ==(const FInventoryItemInfo& InItem) const
	{
		return this->Item == InItem.Item && this->Coordinate == InItem.Coordinate;
	}
	
};

/**
 * Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemAdded, FItemInformation, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryCollectibleItemAdded, class ACollectibleItem*, Item);


class ACollectibleItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETTEMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	
	TArray<FItemInformation> SimulatedInventory;
	bool bIsSimulatedInventoryDirty;
	
protected:
	//Functions, etc...
	UInventoryComponent();

	virtual void OnComponentCreated() override;

	virtual void BeginPlay() override;

public:
	//Functions, etc...

	
	//Adding items//
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventoryItemInfo& InInventoryItemInfo);
	
	void AddItem_Internal(const FInventoryItemInfo& InInventoryItemInfo);

	UFUNCTION(BlueprintCallable)
	bool AddCollectibleItem(ACollectibleItem* InCollectibleItem);
	
	//Removing items//
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(const FInventoryItemInfo& InInventoryItemInfo);

	void RemoveItem_Internal(const FInventoryItemInfo& InInventoryItemInfo);

	UFUNCTION(BlueprintCallable)
	bool RemoveCollectibleItem(ACollectibleItem* InCollectibleItem);

	//Using items//
	UFUNCTION(BlueprintCallable)
	bool UseItem(const FInventoryItemInfo& InInventoryItemInfo);

	
	//Helpers//
	TArray<FItemInformation>& GetSimulatedInventory();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasItem (const FItemInformation& InItemInfo);

	UFUNCTION(BlueprintCallable)
	bool CanFitItemAtCoordinate(const FItemInformation& InItemInfo, const FItemCoordinate& InCoordinate);
	
	UFUNCTION(BlueprintCallable)
	bool CanFitItemSomewhere(const FItemInformation& InItemInfo, FItemCoordinate& OutCoordinate);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FInventoryItemInfo>& GetItems();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ACollectibleItem*> GetCollectibleItems();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FItemCoordinate> GetAllCells();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FInventorySize GetInventorySize();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CoordinateToIndex(const FItemCoordinate& InCoordinate);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemCoordinate IndexToCoordinate(const int32& InIndex);
	
	//Properties
	UPROPERTY()
	TArray<FInventoryItemInfo> InventoryItems;

	UPROPERTY()
	TArray<ACollectibleItem*> CollectibleItems;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInventorySize InventorySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1.0f, UIMin = 1.0f), Category = "Inventory")
	float CellSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDataTableRowHandle> StartupItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ACollectibleItem*> StartupCollectibleItems;

	//Delegates
	UPROPERTY(BlueprintAssignable)	
	FInventoryEvent OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)	
	FInventoryItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)	
	FInventoryCollectibleItemAdded OnCollectibleItemAdded;
	
};


