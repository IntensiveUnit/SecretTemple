// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DraggedItemWidget.h"
#include "UI/GridWidget.h"

void UDraggedItemWidget::SetDraggedItemData(const FInventoryItem& InInventoryItem, UGridWidget* InParentWidget)
{
	InventoryItem = InInventoryItem;
	ParentWidget = InParentWidget;
	
	OnDraggedItemDataReceived();
}


