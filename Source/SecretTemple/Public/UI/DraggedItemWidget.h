// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/STInventoryComponent.h"
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
	void SetDraggedItemData(const FInventoryItem& InInventoryItem, UGridWidget* InParentWidget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DraggedItem")
	void OnDraggedItemDataReceived();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DraggedItem")
	void SetDraggedItemSize(float NewSize);
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "DraggedItem")
	UGridWidget* ParentWidget;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "DraggedItem")
	FInventoryItem InventoryItem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DraggedItem")
	FSlateBrush ValidPlacementColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DraggedItem")
	FSlateBrush InvalidPlacementColor;
	
};
