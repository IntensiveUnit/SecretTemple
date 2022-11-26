// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DraggedItemWidget.h"
#include "UI/GridWidget.h"

void UDraggedItemWidget::SetDraggedItemData(const FInventoryItemSlot& InInventoryItem, UGridWidget* InParentWidget)
{
	InventoryItemInfo = InInventoryItem;
	ParentWidget = InParentWidget;
	
	OnDraggedItemDataReceived();
}


