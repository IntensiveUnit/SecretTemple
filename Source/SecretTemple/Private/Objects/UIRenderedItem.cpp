// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UIRenderedItem.h"

#include "Components/SceneCaptureComponent2D.h"


AUIRenderedItem::AUIRenderedItem(const FObjectInitializer &ObjectInitializer):
Super(ObjectInitializer)
{
	PivotSceneCapture = CreateDefaultSubobject<USceneComponent>("SceneCapturePivot");
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(PivotSceneCapture);
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture2D");
	SceneCaptureComponent2D->SetupAttachment(SpringArmComponent);
}
