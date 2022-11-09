// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Interfaces/InteractInterface.h"
#include "Item.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FItemSize
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", UIMin = "1"))
	int32 Y;

	FItemSize():
	X(1),
	Y(1)
	{}

	FItemSize(const int32 InX, const int32 InY):
	X(InX >= 1 ? InX : 1),
	Y(InY >= 1 ? InY : 1)
	{}

	FItemSize(const FItemSize& InItemSize):
	X(InItemSize.X),
	Y(InItemSize.Y)
	{}

	bool operator ==(const FItemSize& InItemSize) const
	{
		return this->X == InItemSize.X && this->Y == InItemSize.Y;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemInformation : public FTableRowBase
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsConsumable;

	//Might be anything, animation, particle, consumable etc...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSize Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCollectible;
	
	FItemInformation(FString InName, FString InDescription, UTexture2D* InImage,
	          UStaticMesh* InStaticMesh, bool InIsConsumable, TSubclassOf<UGameplayEffect> InItemEffect,
	          bool InCanBeStacked, int32 InAmount, FItemSize InSize, bool InIsCollectible):
		Name(InName),
		Description(InDescription),
		Image(InImage),
		StaticMesh(InStaticMesh),
		IsConsumable(InIsConsumable),
		ItemEffect(InItemEffect),
		CanBeStacked(InCanBeStacked),
		Amount(InAmount),
		Size(InSize),
		IsCollectible(InIsCollectible)
	{
	}

	FItemInformation(const FItemInformation* InItemInfo):
		Name(InItemInfo->Name),
		Description(InItemInfo->Description),
		Image(InItemInfo->Image),
		StaticMesh(InItemInfo->StaticMesh),
		IsConsumable(InItemInfo->IsConsumable),
		ItemEffect(InItemInfo->ItemEffect),
		CanBeStacked(InItemInfo->CanBeStacked),
		Amount(InItemInfo->Amount),
		Size(InItemInfo->Size),
		IsCollectible(InItemInfo->IsCollectible)
	{
	}

	FItemInformation():
	
		Name(""),
		Description(""),
		Image(nullptr),
		StaticMesh(nullptr),
		IsConsumable(false),
		ItemEffect(nullptr),
		CanBeStacked(false),
		Amount(0),
		Size(0,0),
		IsCollectible(false)
	{
	}

	bool operator==(const FItemInformation& ComparisonItem) const
	{
		return this->Name == ComparisonItem.Name &&
			this->Description == ComparisonItem.Description &&
			this->Image == ComparisonItem.Image &&
			this->StaticMesh == ComparisonItem.StaticMesh;
	}

	bool IsEmpty() const
	{
		return this->Name.IsEmpty() && this->Description.IsEmpty();
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangeHasStarted, AItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangeHasEnded, AItem*, Item);

/**
 * Basic representation of the item,
 * contains item information and basic interfaces
 */

UCLASS()
class SECRETTEMPLE_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:
	AItem();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void LoadData();

	FItemInformation ItemInfo;

public:
	//Start implement IInteractionInterface
	virtual bool CanInteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;

	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	//End implement IInteractionInterface

	virtual bool UseItem(APlayerCharacter* InteractionInstigator);

	UFUNCTION(BlueprintCallable)
	void HideItem();
	
	UFUNCTION(BlueprintCallable)
	void SetItemInfo(const FItemInformation& InItemInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemInformation& GetItemInfo();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FDataTableRowHandle DataTableItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* TranslationPivotComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* RotationPivotComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* SnapshotPivotComponent;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemChangeHasStarted OnItemChangeHasStarted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemChangeHasEnded OnItemChangeHasEnded;
};
