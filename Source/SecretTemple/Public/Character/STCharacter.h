// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/STAbilitySystemComponent.h"
#include "Abilities/STAttributeSet.h"
#include "Abilities/STGameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/STInventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "STCharacter.generated.h"

UCLASS()
class SECRETTEMPLE_API ASTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTCharacter(const FObjectInitializer& ObjectInitializer);


protected:

	//TODO clarify WeakPointer
	// Instead of TWeakObjectPtrs, you could just have UPROPERTY() hard references or no references at all and just call
	// GetAbilitySystem() and make a GetAttributeSetBase() that can read from the PlayerState or from child classes.
	// Just make sure you test if the pointer is valid before using.
	// I opted for TWeakObjectPtrs because I didn't want a shared hard reference here and I didn't want an extra function call of getting
	// the ASC/AttributeSet from the PlayerState or child classes every time I referenced them in this base class.

	TWeakObjectPtr<USTAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<USTAttributeSet> AttributeSet;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TArray<TSubclassOf<USTGameplayAbility>> CharacterAbilities;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
	
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
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsRunning;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius;

	UPROPERTY()
	AActor* InteractionActor;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* GunComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* InteractionSphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USTInventoryComponent* InventoryComponent;

	
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
	
	//TODO add gamepad
	//Keyboard
	void Interact();

	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
	
	//Todo clarify
	bool ASCInputBound = false;

	UFUNCTION()
	void OnBeginOverlapInteractionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlapInteractionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddStartupEffects();
	
	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	void AddCharacterAbilities();

	bool IsAlive();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetInteractionActor();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USTInventoryComponent* GetInventory();
	
};

