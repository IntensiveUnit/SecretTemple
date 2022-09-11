// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CellWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/GridSlot.h"
#include "UI/GridWidget.h"
#include "UI/ItemWidget.h"

void UCellWidget::SetCellData(const FIntPoint& InCoordinates, const float InSize, UGridWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
	Coordinates = InCoordinates;
	CellSize = InSize;

	OnCellDataReceived();
}

FReply UCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bMouseWasDragging = false;
		SetCellColor(ClickedColor);
		return FReply::Handled();
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bMouseWasDragging = false;
		SetCellColor(ClickedColor);
		return FReply::Handled();
	}
	else
	{
		bMouseWasDragging = false;
		return FReply::Handled();
	}
}

FReply UCellWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMouseWasDragging = false;
	SetCellColor(LastStateColor);
	return FReply::Handled();
}

void UCellWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetCellColor(HoveredColor);
	LastStateColor = HoveredColor;
}

void UCellWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetCellColor(DefaultColor);
	LastStateColor = DefaultColor;
}

void UCellWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	
	const UDraggedItemWidget* DraggedSlotWidget = Cast<UDraggedItemWidget>(InOperation->DefaultDragVisual);
	for (UCellWidget* CellWidget: ParentWidget->CellWidgets)
	{
		CellWidget->SetCellColor(CellWidget->DefaultColor);
	}

	//TODO Refactor coordinates to coordinate
	if (ParentWidget->Inventory->CanFitItemAtCoordinate(DraggedSlotWidget->InventoryItem.Item, Coordinates))
	{
		for (int I = Coordinates.X; I < Coordinates.X + DraggedSlotWidget->InventoryItem.Item.Size.X; ++I)
		{
			for (int J = Coordinates.Y; J < Coordinates.Y + DraggedSlotWidget->InventoryItem.Item.Size.Y; ++J)
			{
				for (UCellWidget* CellWidget : ParentWidget->CellWidgets)
				{
					if (CellWidget->Coordinates == FIntPoint(I,J))
					{
						CellWidget->SetCellColor(ValidPlacementColor);
						break;
					}
				}
			}
		}
	}
	else
	{
		for (int I = Coordinates.X; I < Coordinates.X + DraggedSlotWidget->InventoryItem.Item.Size.X; ++I)
		{
			for (int J = Coordinates.Y; J < Coordinates.Y + DraggedSlotWidget->InventoryItem.Item.Size.Y; ++J)
			{
				for (UCellWidget* CellWidget : ParentWidget->CellWidgets)
				{
					if (CellWidget->Coordinates == FIntPoint(I,J))
					{
						CellWidget->SetCellColor(InvalidPlacementColor);
						break;
					}
				}
			}
		}
	}
}

void UCellWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	
	for (UCellWidget* CellWidget: ParentWidget->CellWidgets)
	{
		CellWidget->SetCellColor(CellWidget->DefaultColor);
	}
}

void UCellWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	
	for (UCellWidget* CellWidget: ParentWidget->CellWidgets)
	{
		CellWidget->SetCellColor(CellWidget->DefaultColor);
	}
	
}

bool UCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	
	const UDraggedItemWidget* DraggedSlotWidget = Cast<UDraggedItemWidget>(InOperation->DefaultDragVisual);

	for (UCellWidget* CellWidget: ParentWidget->CellWidgets)
	{
		CellWidget->SetCellColor(CellWidget->DefaultColor);
	}

	for (UItemWidget* SlotWidget: ParentWidget->ItemWidgets)
	{
		UGridSlot* GridSlot = UWidgetLayoutLibrary::SlotAsGridSlot(SlotWidget);
		if (GridSlot)
		{
			GridSlot->SetLayer(1);
		}
	}

	if (ParentWidget->Inventory->CanFitItemAtCoordinate(DraggedSlotWidget->InventoryItem.Item, Coordinates))
	{
		ParentWidget->Inventory->AddItemAtCoordinates(DraggedSlotWidget->InventoryItem.Item, Coordinates);
	}
	
	return true;
}