// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Interfaces/InteractInterface.h"
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogItems, Log, All);

UENUM(BlueprintType)
enum EItemType
{
	Inventory UMETA(DisplayName="Inventory item", ShortToolTip="Item that can be put into inventory"),
	Collectible UMETA(DisplayName="Collectible item", ShortToolTip="An item that is used in a separate puzzle menu"),
	Note UMETA(DisplayName="Note item", ShortToolTip="An item that is shown in a separate note's menu"),

	Undefined UMETA(DisplayName="Undefined", ShortToolTip="Item type is not defined")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemWasUsed, AItem*, Item, AActor*, OnWhomItWasUsed, APlayerCharacter*, WhoUsed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangeHasStarted, AItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangeHasEnded, AItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemWasPickedUp, AItem*, Item, APlayerCharacter*, WhoPickedUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemWasDropped, AItem*, Item, APlayerCharacter*, WhoDropped);



/**
 * Shell containing basic functionality
 */

UCLASS(Abstract, NotBlueprintable)
class SECRETTEMPLE_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:
	
	AItem();
	
	/**
	* Item data loading template, since each item has its own structure 
	*/
	template <class T>
	T* LoadData() const
	{
		if (DataTableItem.DataTable.IsNull())
		{
			return nullptr;
		}
	
		if(DataTableItem.RowName.IsNone())
		{
			return nullptr;
		}

		if (!DataTableItem.DataTable->FindRow<T>(DataTableItem.RowName, ""))
		{
			return nullptr;
		}
		
		return DataTableItem.DataTable->FindRow<T>(DataTableItem.RowName, "");
	}
	
	/**
	* Each item type has its own datatable
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetDatatableItem)
	FDataTableRowHandle DataTableItem;

public:

	/**
	* Get row name and datatable item from
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FDataTableRowHandle& GetDatatableItem();

	/**
	* Get item type
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TEnumAsByte<EItemType> GetItemType();
	
	//Start implement IInteractionInterface
	
	virtual bool CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	
	//End implement IInteractionInterface
	
	/**
	* Visible representation of the item in the world
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	/**
	* Root component of an item
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* TranslationPivotComponent;

	/**
	* Used to rotate in the menu
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* RotationPivotComponent;
	
	/**
	* Used to take a screenshot of an item
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* SnapshotPivotComponent;

	/**
	* Called when an item has been used on an actor that can accept items
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemWasUsed FOnItemWasUsed;
	
	/**
	* In case the item plays the animation
	* this delegate is called
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemChangeHasStarted OnItemChangeHasStarted;

	/**
	* In case the item has finished playing
	* animation this delegate is called
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemChangeHasEnded OnItemChangeHasEnded;

	/**
	* Called when an item in a level has been picked up by a player
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemWasPickedUp OnItemWasPickedUp;

	/**
	* Called when an item has been dropped by a player
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemWasDropped FOnItemWasDropped;
	
};
