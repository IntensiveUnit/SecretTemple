// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "CollectibleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SECRETTEMPLE_API UCollectibleWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	

public:

	bool StartRotation;

	UPROPERTY(BlueprintReadOnly)
	FVector2D InitCursorLocation;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D EndCursorLocation;

	
	UFUNCTION(BlueprintImplementableEvent)
	void MouseWheel(float Delta);

	UFUNCTION(BlueprintImplementableEvent)
	void MouseDoubleClick();

	UFUNCTION(BlueprintImplementableEvent)
	void MouseDelta(FVector2D Delta);
};
