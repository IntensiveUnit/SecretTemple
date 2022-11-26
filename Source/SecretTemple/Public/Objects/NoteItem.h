// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Item.h"
#include "NoteItem.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FNoteItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	/**
	 * The note can be divided into several parts, for easier reading, in the note itself we switch between its parts
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> InformationPieces;

	/**
	 * The parent note is the top note in the hierarchy when multiple notes refer to the same
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasParentNote;
	
	/**
	 * Notes that are part of that note and are only shown when opened
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bHasParentNote", EditConditionHides))
	FDataTableRowHandle ParentNote;
	
	/**
	 * Note image displayed behind the text
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;
	
	/**
	 * Visual representation in the world
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
	
	FNoteItemData():
	Name(),
	InformationPieces(),
	bHasParentNote(false),
	ParentNote(FDataTableRowHandle()),
	Image(nullptr),
	StaticMesh(nullptr)
	{}

	FNoteItemData(const FText InName, const TArray<FText> InInformationPieces, const bool InbHasParentNote,
		const FDataTableRowHandle InParentNote, UTexture2D* InImage, UStaticMesh* InStaticMesh):
	Name(InName),
	InformationPieces(InInformationPieces),
	bHasParentNote(InbHasParentNote),
	ParentNote(InParentNote),
	Image(InImage),
	StaticMesh(InStaticMesh)
	{}

	FNoteItemData(const FNoteItemData* InNoteItemData):
		Name(InNoteItemData->Name),
		InformationPieces(InNoteItemData->InformationPieces),
		bHasParentNote(InNoteItemData->bHasParentNote),
		ParentNote(InNoteItemData->ParentNote),
		Image(InNoteItemData->Image),
		StaticMesh(InNoteItemData->StaticMesh)
	{}

	bool DataIsEmpty() const
	{
		return Name.IsEmpty() && InformationPieces.IsEmpty();
	}
};



/**
 * An object used to obtain information, notes, pictures, a journal, etc.
 */
UCLASS(BlueprintType, Blueprintable)
class SECRETTEMPLE_API ANoteItem : public AItem
{
	GENERATED_BODY()

	
	//We make this class friendly because it is the only one capable of spawning items of this type
	//and therefore only it should assign data to them
	friend class UInventoryComponent;
	
protected:
	
	/**
	* Use this method to automatically start loading data from datatable when an item is changed in editor 
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/**
	* Used if we spawn an item from inventory
	*/
	void SetNoteItemData(const FNoteItemData& InNoteItemData);
	
	private:
	/**
	* Each item type has its own data
	*/
	UPROPERTY(BlueprintGetter = GetNoteItemData)
	FNoteItemData NoteItemData;

public:

	/**
	* Get this inventory item's data
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FNoteItemData& GetNoteItemData();

	/**
	* Get item type
	*/
	virtual TEnumAsByte<EItemType> GetItemType() override;
	
	//Start implement IInteractionInterface
	
	virtual bool InteractWithActor_Implementation(APlayerCharacter* InteractionInstigator) override;
	
	//End implement IInteractionInterface
	
};
