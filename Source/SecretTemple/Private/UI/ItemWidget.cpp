// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ItemWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GridSlot.h"
#include "UI/DraggedItemWidget.h"
#include "UI/GridWidget.h"


void UItemWidget::SetItemData(const FInventoryItemInfo& InInventoryItem, UGridWidget* InParentWidget)
{
	InventoryItemInfo = InInventoryItem;
	ParentWidget = InParentWidget;
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FReply Reply = FReply::Handled();
		bMouseWasDragging = false;
		SetColor(ClickedColor);
		OnLeftClick();
		
		const TSharedPtr<SWidget> WidgetDetectingDrag = GetCachedWidget();
		Reply.DetectDrag(WidgetDetectingDrag.ToSharedRef(), EKeys::LeftMouseButton);	
		return Reply;
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bMouseWasDragging = false;
		SetColor(ClickedColor);
		return FReply::Handled();
	}
	else
	{
		bMouseWasDragging = false;
		return FReply::Handled();
	}
}

FReply UItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseWasDragging = false;
	SetColor(LastStateColor);
	OnItemWidgetClicked.Broadcast(this);
	return FReply::Handled();
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetColor(HoveredColor);
	LastStateColor = HoveredColor;
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetColor(DefaultColor);
	LastStateColor = DefaultColor;
}

void UItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	SetColor(DefaultColor);
	LastStateColor = DefaultColor;
	
	const UDraggedItemWidget* DraggedItemWidget = Cast<UDraggedItemWidget>(InOperation->DefaultDragVisual);
	ParentWidget->Inventory->AddItem(FInventoryItemInfo(DraggedItemWidget->InventoryItemInfo.Item, DraggedItemWidget->InventoryItemInfo.Coordinate));
	
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	bMouseWasDragging = true;
	
	UDraggedItemWidget* DraggedSlotWidget = CreateWidget<UDraggedItemWidget>(GetOwningPlayer(), DraggedSlotWidgetClass);
	check(DraggedSlotWidget != nullptr);

	DraggedSlotWidget->SetDraggedItemData(InventoryItemInfo, ParentWidget);
	DraggedSlotWidget->SetDraggedItemSize(ParentWidget->Inventory->CellSize);

	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>(GetOwningPlayer());
	check(DragDropOperation != nullptr);

	DragDropOperation->DefaultDragVisual = DraggedSlotWidget;
	DragDropOperation->Pivot = EDragPivot::TopLeft;

	ParentWidget->Inventory->RemoveItem(FInventoryItemInfo(InventoryItemInfo.Item, InventoryItemInfo.Coordinate));
	OutOperation = DragDropOperation;
}


