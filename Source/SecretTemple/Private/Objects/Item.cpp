// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Item.h"

DEFINE_LOG_CATEGORY(LogItems);

AItem::AItem()
{
	TranslationPivotComponent = CreateDefaultSubobject<USceneComponent>("TranslationPivotComponent");
	SetRootComponent(TranslationPivotComponent);
	
	RotationPivotComponent = CreateDefaultSubobject<USceneComponent>("RotationPivotComponent");
	RotationPivotComponent->SetupAttachment(TranslationPivotComponent);

	SnapshotPivotComponent = CreateDefaultSubobject<USceneComponent>("SnapshotPivotComponent");
	SnapshotPivotComponent->SetupAttachment(RotationPivotComponent);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RotationPivotComponent);
}

FDataTableRowHandle& AItem::GetDatatableItem()
{
	return DataTableItem;
}

TEnumAsByte<EItemType> AItem::GetItemType()
{
	return EItemType::Undefined;
}

bool AItem::CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	//By default, all items can be interacted with
	return true;
}









