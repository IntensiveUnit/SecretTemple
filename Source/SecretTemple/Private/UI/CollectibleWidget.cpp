// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CollectibleWidget.h"

FReply UCollectibleWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MouseWheel(InMouseEvent.GetWheelDelta());
	
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UCollectibleWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	MouseDoubleClick();
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

FReply UCollectibleWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UCollectibleWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	InitCursorLocation = InMouseEvent.GetScreenSpacePosition();
	MouseDelta(InMouseEvent.GetCursorDelta());
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UCollectibleWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EndCursorLocation = InMouseEvent.GetScreenSpacePosition(); 
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}





