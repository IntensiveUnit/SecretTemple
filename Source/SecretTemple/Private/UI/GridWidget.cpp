// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GridWidget.h"

#include "Components/InventoryComponent.h"
#include "UI/CellWidget.h"
#include "UI/ItemWidget.h"

void UGridWidget::SetInventoryData(UInventoryComponent* InInventory)
{
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
	
	ItemWidgets.Empty();
	for (const FInventoryItemSlot& Item: Inventory->GetInventoryItems())
	{
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetOwningPlayer(), ItemWidgetClass);
		
		ItemWidget->SetItemData(Item, CellSize, this);
		
		ItemWidgets.Add(ItemWidget);
		
		OnItemWidgetCreated(ItemWidget);
	}

	CellWidgets.Empty();
	for (const FItemCoordinate& Coordinate: GetAllCells())
	{
		UCellWidget* CellWidget = CreateWidget<UCellWidget>(GetOwningPlayer(), CellWidgetClass);
		
		CellWidget->SetCellData(Coordinate, CellSize, this);
		
		CellWidgets.Add(CellWidget);
		
		OnCellWidgetCreated(CellWidget);
	}
	
}

void UGridWidget::NativeOnInventoryDataReceived()
{
	if (Inventory == nullptr){ return; }
	
	if (!Inventory->OnInventoryUpdated.IsAlreadyBound(this, &ThisClass::OnInventoryUpdated))
	{
		Inventory->OnInventoryUpdated.AddDynamic(this, &ThisClass::OnInventoryUpdated);
	}
	
	OnInventoryUpdated();
}

TArray<FItemCoordinate> UGridWidget::GetAllCells() const
{
	TArray<FItemCoordinate> AllCells;

	for (int I = 0; I < Inventory->GetInventorySize().X; ++I)
	{
		for (int J = 0; J < Inventory->GetInventorySize().Y; ++J)
		{
			AllCells.Add(FItemCoordinate(I, J));
		}
	}

	return AllCells;
}

