
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UGameplayEffect;
class UCustomGameplayAbility;
class UCameraComponent;
class UInventoryComponent;
class UCustomAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionInterrupted);

UCLASS()
class SECRETTEMPLE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

public:
	
	//------------------------------------------------------------------//
	//----------------------Ability system effects and abilities--------//
	//------------------------------------------------------------------//

	// Bind abilities to input via their EAbilityInputID
	void BindASCInput();

	// Add startup effect (Stamina regeneration, etc.)
	void AddStartupEffects();
	
	// Add abilities 
	void AddCharacterAbilities();
	
	//Is character health more than 0
	bool IsAlive();


	
	//------------------------------------------------------------------//
	//----------------------Interaction---------------------------------//
	//------------------------------------------------------------------//

	/**
	* Who is the character interacting with
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	AActor* GetInteractionActor();

	/**
	* Is in interaction with someone in the world
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool GetIsCharacterInteracts();

	/**
	* Assigns an actor with which the character interacts,
	* for example, to move the camera, take control, etc.
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool SetIsCharacterInteracts(AActor* ActorToInteract);

	/**
	* Interrupt the interaction of the actor
	* and call the appropriate delegate
	*/
	void InterruptCharacterInteraction() const;

	//------------------------------------------------------------------//
	//----------------------Movement------------------------------------//
	//------------------------------------------------------------------//

	//TODO description
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMana();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMana();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina();
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetWalkingSpeed();

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRunningSpeed();

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetCrouchingSpeed();
	
	//------------------------------------------------------------------//
	//----------------------Inventory-----------------------------------//
	//------------------------------------------------------------------//

	/**
	* Get the coordinates of the place in front of the character
	* where it is worth spawning the dropped item
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FVector GetItemDropPosition();

	/**
	* Inventory component, stores consumable items,
	* puzzle items, and notes
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent();
	

	
	//------------------------------------------------------------------//
	//----------------------Input---------------------------------------//
	//------------------------------------------------------------------//
	
	// Mouse + Gamepad
	void LookUp(float Value);
	
	// Mouse + Gamepad
	void Turn(float Value);

	// KeyBoard + Gamepad
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveForward(float Value);

	// KeyBoard + Gamepad
	void MoveRight(float Value);
	
	//Keyboard
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();


	
	//------------------------------------------------------------------//
	//----------------------Properties----------------------------------//
	//------------------------------------------------------------------//
	public:
	/**
	* Character camera component
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;
	
	/**
	* Gameplay ability system component
	*/
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	
	/**
	* Gameplay ability attribute set
	*/
	UPROPERTY()
	UCustomAttributeSet* AttributeSet;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	UAbilitySystemComponent* GetAbilitySystemComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	UCustomAttributeSet* GetAttributeSet();
	
	/**
	* Default abilities for this Character
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UCustomGameplayAbility>> CharacterAbilities;

	/**
	* These effects are only applied one time on startup
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	/**
	* Turn left right speed
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float BaseTurnRate = 45.0f;

	/**
	* Turn up down speed
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float BaseLookUpRate = 45.0f;
	
	/**
	* Radius in which the character can interact with objects
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	float InteractionRadius;
	
	UFUNCTION(BlueprintPure)
	float GetMoveForwardInputScalar();

private:

	UPROPERTY(BlueprintGetter=GetMoveForwardInputScalar)
	float MoveForwardInputScalar;
	
	/**
	* Inventory component, stores consumable items, puzzle items, and notes
	*/
	UPROPERTY(BlueprintGetter=GetInventoryComponent)
	UInventoryComponent* InventoryComponent;

	/**
	* The actor with which the character interacts
	*/
	UPROPERTY()
	AActor* InteractionActor;

	/**
	* Means that the character is in interaction with some kind of entity, for example, in the object's inspection menu
	*/
	UPROPERTY()
	bool IsCharacterInteracts;

	
	//------------------------------------------------------------------//
	//----------------------Delegates-----------------------------------//
	//------------------------------------------------------------------//

	/**
	* If it is urgent to interrupt the interaction of the character with something,
	* for example, while the player was solving a puzzle and a monster attacked him, causing a QTE event
	*/
	UPROPERTY(BlueprintAssignable)	
	FInteractionInterrupted OnInteractionInterrupted;
};