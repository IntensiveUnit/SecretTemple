// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Item.h"
#include "PlayerState/CustomPlayerState.h"

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



void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadData();
}

bool AItem::CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	return IInteractInterface::CanInteractWithActor_Implementation(InteractionInstigator);
}

bool AItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	return IInteractInterface::InteractWithActor_Implementation(InteractionInstigator);
}

bool AItem::UseItem(APlayerCharacter* InteractionInstigator)
{
	return true;
}

void AItem::HideItem()
{
	StaticMeshComponent->SetSimulatePhysics(false);
	SetActorLocation(FVector(0, 0, -9000));
}

void AItem::LoadData()
{
	//TODO make cache for repeated calls
	if (DataTableItem.DataTable.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("No DataTable in %s"), *this->GetName());
		return;
	}
	
	if(DataTableItem.RowName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("No DataTable name in %s"), *this->GetName());
		return;
	}

	if (!DataTableItem.DataTable->FindRow<FItemInformation>(DataTableItem.RowName, ""))
	{
		UE_LOG(LogTemp, Warning, TEXT("No data in DataTable in row %s, Datatable corrupt"), *DataTableItem.RowName.ToString());
		return;
	}
	
	ItemInfo = DataTableItem.DataTable->FindRow<FItemInformation>(DataTableItem.RowName, "");
	
	if (!ItemInfo.StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("No static mesh in item %s"), *ItemInfo.Name);
		return;
	}
	
	StaticMeshComponent->SetStaticMesh(ItemInfo.StaticMesh);
}

void AItem::SetItemInfo(const FItemInformation& InItemInfo)
{
	ItemInfo = InItemInfo;
}

FItemInformation& AItem::GetItemInfo()
{
	LoadData();
	return ItemInfo;
}





