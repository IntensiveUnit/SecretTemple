// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

UCLASS()
class SECRETTEMPLE_API UCustomBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
 	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Editor Camera Location", Keywords = "Get Editor Camera Location"))
 	static FVector GetEditorCameraLocation();
 	
 	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Editor Camera Direction", Keywords = "Get Editor Camera Direction"))
 	static FVector GetEditorCameraFacingDirection();
 	
 	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Editor Camera Rotation", Keywords = "Get Editor Camera Rotation"))
 	static FRotator GetEditorCameraRotation();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Game Viewport Size", Keywords = "Get Game Viewport Size"))
	static FVector2D GetGameViewportSize();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Game Resolution", Keywords = "Get Game Resolution"))
	static FVector2D GetGameResolution();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Index To Inventory Coordinate", Keywords = "Index To Inventory Coordinate"))
	static FItemCoordinate IndexToCoordinate(const int32& InIndex, const FInventorySize& InInventorySize);
	
};
