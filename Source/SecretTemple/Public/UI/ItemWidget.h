// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "DraggedItemWidget.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class UGridWidget;
class UItemWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemWidgetClicked, UItemWidget*, ItemWidget);

/**
 * USlotWidget
 */
UCLASS()
class SECRETTEMPLE_API UItemWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetItemData(const FInventoryItemInfo& InInventoryItem, UGridWidget* InParentWidget);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Slot")
	void SetSize(const float NewSize);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Slot")
	void SetColor(const FSlateBrush& NewColor);
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Slot")
	void OnLeftClick();

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Slot")
	UGridWidget* ParentWidget;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Slot", meta = (ExposeOnSpawn=true))
	FInventoryItemInfo InventoryItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	FSlateBrush DefaultColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	FSlateBrush HoveredColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	FSlateBrush ClickedColor;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Slot")
	FSlateBrush LastStateColor;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Slot")
	uint8 bMouseWasDragging : 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	TSubclassOf<UDraggedItemWidget> DraggedSlotWidgetClass;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemWidgetClicked OnItemWidgetClicked;
	
};
