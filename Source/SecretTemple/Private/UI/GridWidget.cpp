// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GridWidget.h"

#include "Components/InventoryComponent.h"
#include "UI/CellWidget.h"
#include "UI/ItemWidget.h"

void UGridWidget::SetInventoryData(UInventoryComponent* InInventory)
{
	OnPrePopulateData();
	
	Inventory = InInventory;
	NativeOnInventoryDataReceived();
	
}

void UGridWidget::OnInventoryUpdated()
{
	
	//Allow blueprints to remove existing slots from the grid before clearing slots array
	for (UItemWidget* SlotWidget: ItemWidgets)
	{
		OnItemWidgetRemoved(SlotWidget);
	}
	
	//TODO instead of full deleting and adding write function which add item widgets at needed locations(in this case we might use animations and etc)
	ItemWidgets.Empty();
	
	for (const FInventoryItemInfo& Item: Inventory->GetItems())
	{
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetOwningPlayer(), ItemWidgetClass);
		check(ItemWidget != nullptr);
		ItemWidget->SetItemData(Item, this);
		ItemWidgets.Add(ItemWidget);
		OnItemWidgetCreated(ItemWidget);
	}
	
}

void UGridWidget::NativeOnInventoryDataReceived()
{
	if (Inventory == nullptr){ return; }
	
	if (!Inventory->OnInventoryUpdated.IsAlreadyBound(this, &ThisClass::OnInventoryUpdated))
	{
		Inventory->OnInventoryUpdated.AddDynamic(this, &ThisClass::OnInventoryUpdated);
	}
	
	CellWidgets.Empty();
	for (const FItemCoordinate Coordinate: Inventory->GetAllCells())
	{
		UCellWidget* CellWidget = CreateWidget<UCellWidget>(GetOwningPlayer(), CellWidgetClass);
		check(CellWidget != nullptr);

		CellWidget->SetCellData(Coordinate, Inventory->CellSize, this);
		
		CellWidgets.Add(CellWidget);
		OnCellWidgetCreated(CellWidget);
	}

	OnInventoryUpdated();
	
}

