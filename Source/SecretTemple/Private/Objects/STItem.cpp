// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/STItem.h"
#include "Character/STCharacter.h"
#include "PlayerState/STPlayerState.h"


ASTItem::ASTItem(const FObjectInitializer &ObjectInitializer):
Super(ObjectInitializer)
{
	PivotStaticMesh = CreateDefaultSubobject<USceneComponent>("PivotStaticMesh");
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	
	SkeletalMeshComponent->SetupAttachment(PivotStaticMesh);
}

void ASTItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (ItemInfo.IsEmpty())
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
		
		ItemInfo = DataTableItemName.DataTable->FindRow<FItem>(DataTableItemName.RowName, "");
	}
	
	if (!ItemInfo.StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh for %s in, actor %s will be invisible"), *DataTableItemName.RowName.ToString(), *this->GetName());
	}

	//SkeletalMeshComponent->SetSkeletalMesh(ItemStaticMesh);
}

bool ASTItem::UseItem(const ASTCharacter* InteractionInstigator)
{
	if (!ItemInfo.ItemEffect) { return false; }
	
	if (!InteractionInstigator->GetPlayerState()) { return false;}
	const ASTPlayerState* PlayerState = Cast<ASTPlayerState>(InteractionInstigator->GetPlayerState());

	if (!PlayerState) { return false; }

	if (!PlayerState->GetAbilitySystemComponent()) { return false; }
//	PlayerState->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(ItemInfo.ItemEffect);
	
	
	return true;
}

void ASTItem::SetItemInfo(const FItem& InItem)
{
	ItemInfo = InItem;
}

FItem& ASTItem::GetItemInfo()
{
	return ItemInfo;
}

bool ASTItem::CanInteract_Implementation(ASTCharacter* InteractionInstigator)
{
	return true;
}

bool ASTItem::Interact_Implementation(ASTCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventory()->AddItem(ItemInfo);

	if (bWasAdded)
	{
		Destroy();
	}
	
	return bWasAdded;
}