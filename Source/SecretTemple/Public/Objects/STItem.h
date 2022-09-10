// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/STGameState.h"
#include "Engine/DataTable.h"
#include "Interfaces/InteractInterface.h"
#include "UObject/NoExportTypes.h"
#include "STItem.generated.h"

class UGameplayEffect;

//TODO Do something with converting static meshes to skeletal meshes
USTRUCT(BlueprintType, Blueprintable)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayEffect* ItemEffect;
	

private:
	bool bIsEmpty;

public:
	FItem(FString InName, FString InDescription, bool InCanBeStacked, int32 InAmount, FIntPoint InSize, UTexture2D* InImage,
		UStaticMesh* InStaticMesh, UGameplayEffect* InItemEffect):
	Name(InName),
	Description(InDescription),
	CanBeStacked(InCanBeStacked),
	Amount(InAmount),
	Size(InSize),
	Image(InImage),
	StaticMesh(InStaticMesh),
	ItemEffect(InItemEffect),
	bIsEmpty(false)
	{}

	FItem():
	Name(TEXT("")),
	Description(TEXT("")),
	CanBeStacked(false),
	Amount(0),
	Size(0,0),
	Image(nullptr),
	StaticMesh(nullptr),
	ItemEffect(nullptr),
	bIsEmpty(true)
	{}

	FItem(FItem* InItem):
	Name(InItem->Name),
	Description(InItem->Description),
	CanBeStacked(false),
	Amount(InItem->Amount),
	Size(InItem->Size),
	Image(InItem->Image),
	StaticMesh(InItem->StaticMesh),
	ItemEffect(InItem->ItemEffect), 
	bIsEmpty(false)
	{}

	bool operator==(const FItem &ComparisonItem) const
	{
		return this->Name == ComparisonItem.Name ;
	}
	
	bool IsEmpty() const
	{
		return this->Name.IsEmpty();
	}
};


/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API ASTItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:
	
	ASTItem(const FObjectInitializer &ObjectInitializer);
	
	/*
	* C/C++ compiler will put jump to constructor on any point that object is initiated right away (when “new” operator is used),
	* so UE4 don’t even have a chance to update data for constructor execution, that's how C/C++ works. It needs to do so after that point.
	* Easiest event you can use is PostInitializeComponents because he fires up and in PIE and in Editor construction:
	* the construction of the actor, this is where Blueprint actors have their components created and blueprint variables are initialized
	* https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Actors/ActorLifecycle/
	*/
	virtual void PostInitializeComponents() override;

	
public:
	//Functions, etc...
	
	
	bool UseItem(const ASTCharacter* InteractionInstigator);

	virtual bool CanInteract_Implementation(ASTCharacter* InteractionInstigator) override;

	virtual bool Interact_Implementation(ASTCharacter* InteractionInstigator) override;
	
	FItem ItemInfo;

	UFUNCTION(BlueprintCallable)
	void SetItemInfo (const FItem& InItem);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItem& GetItemInfo();
	
	//Properties
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDataTableRowHandle DataTableItemName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* PivotStaticMesh;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	
	
};
