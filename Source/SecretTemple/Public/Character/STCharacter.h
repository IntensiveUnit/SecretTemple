// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "STCharacter.generated.h"

UCLASS()
class SECRETTEMPLE_API ASTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	float StartingCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	FVector StartingCameraBoomLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	TArray<FName> HiddenBones;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* GunComponent;
	
	// Mouse + Gamepad
	void LookUp(float Value);
	
	// Mouse + Gamepad
	void Turn(float Value);

	// KeyBoard + Gamepad
	void MoveForward(float Value);

	// KeyBoard + Gamepad
	void MoveRight(float Value);

	//TODO add gamepad
	//Keyboard
	void ToggleCrouch();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

