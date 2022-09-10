// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	
	void NativeOnInventoryDataReceived();
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetInventoryData(USTInventoryComponent* InInventory);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnPrePopulateData();

	UFUNCTION()
	void OnInventoryUpdated();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnCellWidgetCreated(UCellWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnItemWidgetCreated(UItemWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnItemWidgetRemoved(UItemWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void OnCellWidgetRemoved(UCellWidget* Widget);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<UCellWidget> CellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	TArray<UItemWidget*> ItemWidgets;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	TArray<UCellWidget*> CellWidgets;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Grid")
	USTInventoryComponent* Inventory;
};
