// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Objects/STItem.h"
#include "UIRenderedItem.generated.h"

/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API AUIRenderedItem : public ASTItem
{
	GENERATED_BODY()
protected:
	AUIRenderedItem(const FObjectInitializer &ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PivotSceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2D;;

	
};
