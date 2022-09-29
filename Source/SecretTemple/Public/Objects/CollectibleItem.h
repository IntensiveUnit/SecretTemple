// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STItem.h"

#include "CollectibleItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Blueprintable)
struct FCollectibleItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	
	FCollectibleItemStruct(FString InName, FString InDescription, UTexture2D* InImage,
		UStaticMesh* InStaticMesh):
	Name(InName),
	Description(InDescription),
	Image(InImage),
	StaticMesh(InStaticMesh)
	{}

	FCollectibleItemStruct():
	Name(TEXT("")),
	Description(TEXT("")),
	Image(nullptr),
	StaticMesh(nullptr)
	{}

	FCollectibleItemStruct(const FCollectibleItemStruct* InCollectibleItem):
	Name(InCollectibleItem->Name),
	Description(InCollectibleItem->Description),
	Image(InCollectibleItem->Image),
	StaticMesh(InCollectibleItem->StaticMesh)
	{}

	bool operator==(const FCollectibleItemStruct &ComparisonItem) const
	{
		return this->Name == ComparisonItem.Name ;
	}

	bool IsEmpty() const
	{
		return this->Name.IsEmpty();
	}
	
};

UCLASS()
class SECRETTEMPLE_API ACollectibleItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:
	bool bWasResolved;
	
	ACollectibleItem(const FObjectInitializer &ObjectInitializer);
	
	/*
	* C/C++ compiler will put jump to constructor on any point that object is initiated right away (when “new” operator is used),
	* so UE4 don’t even have a chance to update data for constructor execution, that's how C/C++ works. It needs to do so after that point.
	* Easiest event you can use is PostInitializeComponents because he fires up and in PIE and in Editor construction:
	* the construction of the actor, this is where Blueprint actors have their components created and blueprint variables are initialized
	* https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Actors/ActorLifecycle/
	*/
	virtual void PostInitializeComponents() override;

	FCollectibleItemStruct CollectibleItemInfo;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWasResolved(bool InWasResolved);

	UFUNCTION(BlueprintCallable)
	bool GetWasResolved();

	virtual bool CanInteract_Implementation(ASTCharacter* InteractionInstigator) override;

	virtual bool Interact_Implementation(ASTCharacter* InteractionInstigator) override;
	
	UFUNCTION(BlueprintCallable)
	void SetItemInfo (const FCollectibleItemStruct& InCollectibleItemInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FCollectibleItemStruct& GetItemInfo();
	
	//Properties
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDataTableRowHandle DataTableItemName;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* SceneComponent;
};





