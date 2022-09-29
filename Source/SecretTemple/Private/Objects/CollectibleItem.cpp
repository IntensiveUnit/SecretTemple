// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CollectibleItem.h"

#include "Character/STCharacter.h"

ACollectibleItem::ACollectibleItem(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(SceneComponent);
}
//TODO How ca i assign static mesh via datatable, so he visible in editor? not only after begin play
void ACollectibleItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (CollectibleItemInfo.IsEmpty())
	{
		/*Standard UDatatable::IsNull() standard does not useful
		*If we reset the value in the blueprint, then the name is saved, and the table is deleted
		*But the check in this method uses AND, not OR, and the engine crashes.
		*/
		if (!DataTableItemName.DataTable || DataTableItemName.RowName.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("No DataTable name in %s"), *this->GetName());
			return;
		}
		//TODO this need be instance or reference?
		CollectibleItemInfo = DataTableItemName.DataTable->FindRow<FCollectibleItemStruct>(DataTableItemName.RowName, "");
	}
	
	if (!CollectibleItemInfo.StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("No static mesh for %s in, actor %s will be invisible"), *DataTableItemName.RowName.ToString(), *this->GetName());
	}

	StaticMeshComponent->SetStaticMesh(CollectibleItemInfo.StaticMesh);

	
}

void ACollectibleItem::SetWasResolved(bool InWasResolved)
{
	bWasResolved = InWasResolved;
}

bool ACollectibleItem::GetWasResolved()
{
	return bWasResolved;
}

bool ACollectibleItem::CanInteract_Implementation(ASTCharacter* InteractionInstigator)
{
	return true;
}

bool ACollectibleItem::Interact_Implementation(ASTCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventory()->AddCollectibleItemToInventory(this);

	if (bWasAdded)
	{
		//TODO rewrite
		SetActorLocation(FVector(0, 0 , -9000.f));
		
	}
	
	return bWasAdded;
}

void ACollectibleItem::SetItemInfo(const FCollectibleItemStruct& InCollectibleItemInfo)
{
	CollectibleItemInfo = InCollectibleItemInfo;
}

FCollectibleItemStruct& ACollectibleItem::GetItemInfo()
{
	return CollectibleItemInfo;
}
