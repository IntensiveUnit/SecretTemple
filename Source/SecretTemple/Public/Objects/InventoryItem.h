// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Item.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API AInventoryItem : public AItem
{
	GENERATED_BODY()

public:
	
	virtual bool CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;

	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;

	virtual bool UseItem(APlayerCharacter* InteractionInstigator) override;
};
