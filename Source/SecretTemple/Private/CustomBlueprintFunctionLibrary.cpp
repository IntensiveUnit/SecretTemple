// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBlueprintFunctionLibrary.h"
#include "EditorViewportClient.h"
#include "Components/InventoryComponent.h"

FVector UCustomBlueprintFunctionLibrary::GetEditorCameraLocation() {
	const FEditorViewportClient* Client = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	const FVector EditorCameraLocation = Client->GetViewLocation();
	return EditorCameraLocation;
}
FVector UCustomBlueprintFunctionLibrary::GetEditorCameraFacingDirection() {
	const FEditorViewportClient* Client = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	const FVector EditorCameraRotation = Client->GetViewRotation().Vector();
	return EditorCameraRotation;
}
FRotator UCustomBlueprintFunctionLibrary::GetEditorCameraRotation() {
	const FEditorViewportClient* Client = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	const FRotator EditorCameraRotation = Client->GetViewRotation();
	return EditorCameraRotation;
}

FVector2D UCustomBlueprintFunctionLibrary::GetGameViewportSize()
{
	FVector2D Result = FVector2D( 1, 1 );

	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize( /*out*/Result );
	}

	return Result;
}

FVector2D UCustomBlueprintFunctionLibrary::GetGameResolution()
{
	FVector2D Result = FVector2D( 1, 1 );

	Result.X = GSystemResolution.ResX;
	Result.Y = GSystemResolution.ResY;

	return Result;
}

FItemCoordinate UCustomBlueprintFunctionLibrary::IndexToCoordinate(const int32& InIndex,
	const FInventorySize& InInventorySize)
{
	if (InInventorySize.X <= 0 || InInventorySize.Y <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't convert index in coordinate, inventory size contains value less or equal zero")) return FItemCoordinate(0,0);
	}

	if (InIndex < 0 || InIndex > InInventorySize.X * InInventorySize.Y - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't convert index in coordinate, index value less or bigger than matrix parameter")) return FItemCoordinate(0,0);
	}
	
	int32 const Row = FMath::RoundToNegativeInfinity(static_cast<float>(InIndex) / InInventorySize.X);
	int32 const Column = InIndex % InInventorySize.X;
	return FItemCoordinate(FMath::Clamp(Row, 0, InInventorySize.X), FMath::Clamp(Column, 0, InInventorySize.Y));
}


