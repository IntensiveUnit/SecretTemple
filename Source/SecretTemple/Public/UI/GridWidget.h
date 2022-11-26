// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "GridWidget.generated.h"

class USTInventoryComponent;
class UItemWidget;
class UCellWidget;
/**
 * UGridWidget
 */
UCLASS()
class SECRETTEMPLE_API UGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetInventoryData(UInventoryComponent* InInventory);
	
	void NativeOnInventoryDataReceived();
	
	UFUNCTION()
	void OnInventoryUpdated();

	//------------------------------------------------------------------//
	//----------------------Widgets created-----------------------------//
	//------------------------------------------------------------------//
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnCellWidgetCreated(UCellWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnItemWidgetCreated(UItemWidget* Widget);

	//------------------------------------------------------------------//
	//----------------------Widgets removed-----------------------------//
	//------------------------------------------------------------------//
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnItemWidgetRemoved(UItemWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnCellWidgetRemoved(UCellWidget* Widget);

	//------------------------------------------------------------------//
	//----------------------Properties----------------------------------//
	//------------------------------------------------------------------//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<UCellWidget> CellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<UItemWidget> ItemWidgetClass;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	TArray<UItemWidget*> ItemWidgets;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	TArray<UCellWidget*> CellWidgets;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	UInventoryComponent* Inventory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1.0f, UIMin = 1.0f), Category = "Inventory")
	float CellSize;

	//------------------------------------------------------------------//
	//----------------------Helpers-------------------------------------//
	//------------------------------------------------------------------//

	TArray<FItemCoordinate> GetAllCells() const;
};
