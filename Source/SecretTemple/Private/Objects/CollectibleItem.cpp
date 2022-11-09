// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CollectibleItem.h"

#include "Character/PlayerCharacter.h"


bool ACollectibleItem::UseItem(APlayerCharacter* InteractionInstigator)
{
	return Super::UseItem(InteractionInstigator);
}

void ACollectibleItem::SetWasResolved(bool InWasResolved)
{
	bWasResolved = InWasResolved;
}

bool ACollectibleItem::GetWasResolved()
{
	return bWasResolved;
}

bool ACollectibleItem::CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	return true;
}

bool ACollectibleItem::InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator)
{
	bool const bWasAdded = InteractionInstigator->GetInventory()->AddCollectibleItem(this);

	if (bWasAdded)
	{
		//TODO rewrite
		SetActorLocation(FVector(0, 0 , -9000.f));
		OnCollectibleItemPickuped.Broadcast(this);
	}
	
	return bWasAdded;
}




