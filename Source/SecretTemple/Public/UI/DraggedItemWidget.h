// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "DraggedItemWidget.generated.h"


class UGridWidget;

/**
 * UDraggedItemWidget
 */
UCLASS()
class SECRETTEMPLE_API UDraggedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "DraggedItem")
	void SetDraggedItemData(const FInventoryItemInfo& InInventoryItem, UGridWidget* InParentWidget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DraggedItem")
	void OnDraggedItemDataReceived();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DraggedItem")
	void SetDraggedItemSize(float NewSize);
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "DraggedItem")
	UGridWidget* ParentWidget;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "DraggedItem", meta = (ExposeOnSpawn=true))
	FInventoryItemInfo InventoryItemInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DraggedItem")
	FSlateBrush ValidPlacementColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DraggedItem")
	FSlateBrush InvalidPlacementColor;
	
};
