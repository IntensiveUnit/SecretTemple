// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CollectibleItem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectibleItemPickuped, class ACollectibleItem*, InItem);

/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API ACollectibleItem : public AItem
{
	GENERATED_BODY()

protected:
	
	bool bWasResolved;

public:
	
	//Start implement IInteractionInterface
	virtual bool CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;

	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	//End implement IInteractionInterface

	virtual bool UseItem(APlayerCharacter* InteractionInstigator) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWasResolved(bool InWasResolved);

	UFUNCTION(BlueprintCallable)
	bool GetWasResolved();

	UPROPERTY(BlueprintAssignable)
	FOnCollectibleItemPickuped OnCollectibleItemPickuped;
	
};





