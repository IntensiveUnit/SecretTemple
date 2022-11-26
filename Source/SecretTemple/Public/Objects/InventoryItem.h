// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Item.h"
#include "InventoryItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

USTRUCT(BlueprintType, Blueprintable)
struct FItemSize
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", UIMin = "1"))
	int32 Y;

	FItemSize():
	X(1),
	Y(1)
	{}

	FItemSize(const int32 InX, const int32 InY):
	X(InX >= 1 ? InX : 1),
	Y(InY >= 1 ? InY : 1)
	{}

	FItemSize(const FItemSize& InItemSize):
	X(InItemSize.X),
	Y(InItemSize.Y)
	{}

	bool operator ==(const FItemSize& InItemSize) const
	{
		return this->X == InItemSize.X && this->Y == InItemSize.Y;
	}
};



USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsConsumable;

	//Might be anything, animation, particle, consumable etc...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSize Size;
	
	
	FInventoryItemData(FText InName, FText InDescription, UTexture2D* InImage,
	          UStaticMesh* InStaticMesh, bool InIsConsumable, TSubclassOf<UGameplayEffect> InItemEffect,
	          bool InCanBeStacked, int32 InAmount, FItemSize InSize):
		Name(InName),
		Description(InDescription),
		Image(InImage),
		StaticMesh(InStaticMesh),
		IsConsumable(InIsConsumable),
		ItemEffect(InItemEffect),
		CanBeStacked(InCanBeStacked),
		Amount(InAmount),
		Size(InSize)
	{
	}

	FInventoryItemData(const FInventoryItemData* InItemInfo):
		Name(InItemInfo->Name),
		Description(InItemInfo->Description),
		Image(InItemInfo->Image),
		StaticMesh(InItemInfo->StaticMesh),
		IsConsumable(InItemInfo->IsConsumable),
		ItemEffect(InItemInfo->ItemEffect),
		CanBeStacked(InItemInfo->CanBeStacked),
		Amount(InItemInfo->Amount),
		Size(InItemInfo->Size)
	{
	}

	FInventoryItemData():
	
		Name(),
		Description(),
		Image(nullptr),
		StaticMesh(nullptr),
		IsConsumable(false),
		ItemEffect(nullptr),
		CanBeStacked(false),
		Amount(0),
		Size(0,0)
	{
	}

	bool operator==(const FInventoryItemData& ItemForComparison) const
	{
		return this->Name.EqualTo(ItemForComparison.Name) &&
			this->Description.EqualTo(ItemForComparison.Description) &&
			this->Image == ItemForComparison.Image;
	}

	bool IsEmpty() const
	{
		return this->Name.IsEmpty() && this->Description.IsEmpty();
	}
};
/**
 * An item that can be moved into the inventory and is shown in the cell inventory
 */
UCLASS()
class SECRETTEMPLE_API AInventoryItem : public AItem
{
	GENERATED_BODY()

	//We make this class friendly because it is the only one capable of spawning items of this type
	//and therefore only it should assign data to them
	friend class UInventoryComponent;
	
protected:
	/**
	* Use this method to automatically start loading data from datatable when an item is changed in editor 
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/**
	* Used if we spawn an item 
	*/
	void SetInventoryItemData(const FInventoryItemData& InInventoryItemData);
	
private:
	/**
	* Each item type has its own data
	*/
	UPROPERTY(BlueprintGetter = GetInventoryItemData)
	FInventoryItemData InventoryItemData;
	
public:
	
	/**
	* Get this inventory item's data
	*/
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FInventoryItemData& GetInventoryItemData();

	/**
	* Get item type
	*/
	virtual TEnumAsByte<EItemType> GetItemType() override;
	
	//Start implement IInteractionInterface
	
	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	
	//End implement IInteractionInterface
	
	virtual bool UseItem(APlayerCharacter* InteractionInstigator);
};
