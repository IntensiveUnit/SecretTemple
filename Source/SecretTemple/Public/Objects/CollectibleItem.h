// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CollectibleItem.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FCollectibleItemData : public FTableRowBase
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
	
	FCollectibleItemData(FText InName, FText InDescription, UTexture2D* InImage,
	          UStaticMesh* InStaticMesh):
		Name(InName),
		Description(InDescription),
		Image(InImage),
		StaticMesh(InStaticMesh)
	{
	}

	FCollectibleItemData(const FCollectibleItemData* InCollectibleItemData):
		Name(InCollectibleItemData->Name),
		Description(InCollectibleItemData->Description),
		Image(InCollectibleItemData->Image),
		StaticMesh(InCollectibleItemData->StaticMesh)
	{
	}

	FCollectibleItemData():
	
		Name(),
		Description(),
		Image(nullptr),
		StaticMesh(nullptr)
	{
	}

	bool operator==(const FCollectibleItemData& CollectibleItemForComparison) const
	{
		if (this->Name.IsEmpty() || CollectibleItemForComparison.Name.IsEmpty()) return false;
		if (this->Description.IsEmpty() || CollectibleItemForComparison.Description.IsEmpty()) return false;
		if (!this->Image || !CollectibleItemForComparison.Image) return false;
		
		return this->Name.EqualTo(CollectibleItemForComparison.Name)  &&
			this->Description.EqualTo(CollectibleItemForComparison.Description) &&
			this->Image == CollectibleItemForComparison.Image;
	}

	bool DataIsEmpty() const
	{
		return this->Name.IsEmpty() && this->Description.IsEmpty();
	}
};


/**
 * Items used to solve various puzzles
 */
UCLASS()
class SECRETTEMPLE_API ACollectibleItem : public AItem
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
	* Used if we spawn an item from inventory
	*/
	void SetCollectibleItemData(const FCollectibleItemData& InCollectibleItemData);
	//TODO why i see this property without getter in blueprints?
private:
	/**
	* Each item type has its own data
	*/
	UPROPERTY(BlueprintGetter = GetCollectibleItemData)
	FCollectibleItemData CollectibleItemData;

public:

	/**
	* Get this inventory item's data
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FCollectibleItemData& GetCollectibleItemData();

	/**
	* Get item type
	*/
	virtual TEnumAsByte<EItemType> GetItemType() override;
	
	//Start implement IInteractionInterface
	
	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	
	//End implement IInteractionInterface
	
	
};





