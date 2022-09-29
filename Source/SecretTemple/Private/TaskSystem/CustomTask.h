// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaticMulticastDelegate, class ACustomTask*, Task);
/**
 * 
 */
UCLASS()
class ACustomTask : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FStaticMulticastDelegate OnCompleted_Delegate;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent )
	void OnActivated();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent )
	void OnCompleted();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent )
	void OnAfterCompleted();
};
