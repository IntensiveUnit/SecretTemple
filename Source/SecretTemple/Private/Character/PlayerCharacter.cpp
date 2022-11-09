// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Abilities/CustomGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/CustomMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/InteractInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerState/CustomPlayerState.h"
#include "SecretTemple/SecretTemple.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(CharacterMovementComponentName)),
	InteractionRadius(300.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(GetCapsuleComponent());
	FollowCamera->bUsePawnControlRotation = true;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	ToolComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);

	//TODO make this via Datatable
	ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>();
	if (PS)
	{
		// Set the ASC
		AbilitySystemComponent = PS->GetAbilitySystemComponent();

		if (PS->GetAbilitySystemComponent())
		{
			//We do this to make sure that all the information about the actor will definitely be in the abilities
			PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		}
		
		
	}
	
	AddStartupEffects();
	
	AddCharacterAbilities();
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*
	if (ToolComponent && GetMesh())
	{
		ToolComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("ToolSocket"));
	}
	*/
	
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	
	UCustomMovementComponent* CustomPawnMovementComponent = Cast<UCustomMovementComponent>(PawnMovementComponent);
	if (!CustomPawnMovementComponent) { return; }

	CustomPawnMovementComponent->MaxFlySpeed = CustomPawnMovementComponent->MaxWalkSpeedCrouched;
	CustomPawnMovementComponent->BrakingDecelerationFlying = 2000.f;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const UWorld* World = GetWorld();

	if (!World)
	{
		InteractionActor = nullptr;
		return;
	}

	if (IsCharacterInteracts){ return; }
	
	FHitResult HitResult;
	
	if(!World->SweepSingleByChannel(HitResult, FollowCamera->GetComponentLocation(),
		FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * InteractionRadius,
		FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(5.f)))
	{
		InteractionActor = nullptr;
		return;
	}
	DrawDebugSphere(GetWorld(),HitResult.Location, 10.f, 16, InteractionActor ? FColor::Green : FColor::Red);
	if (!HitResult.GetActor())
	{
		InteractionActor = nullptr;
		return;
	}
	
	if (!HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		InteractionActor = nullptr;
		return;
	}
	
	InteractionActor = HitResult.GetActor();
	
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn Right / Left", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &APlayerCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	
	//TODO clarify
	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void APlayerCharacter::LookUp(float Value)
{
	if (IsCharacterInteracts){ return; }
	
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
}

void APlayerCharacter::Turn(float Value)
{
	if (IsCharacterInteracts){ return; }
	
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

void APlayerCharacter::MoveForward_Implementation(float Value)
{
	if (Value == 0){ return; }
	
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }

	//TODO rewrite this when ladder will be Ability via GameplayAbility
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	
	UCustomMovementComponent* CustomPawnMovementComponent = Cast<UCustomMovementComponent>(PawnMovementComponent);
	if (!CustomPawnMovementComponent) { return; }
	
	if (CustomPawnMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::DotProduct(FollowCamera->GetForwardVector(), GetActorUpVector()));
		FVector MovementVector = FVector::DotProduct(FollowCamera->GetForwardVector(), GetActorUpVector()) > -0.5f ? GetActorUpVector() : GetActorUpVector() * -1 + FollowCamera->GetForwardVector();
		MovementVector.Normalize();
		AddMovementInput(MovementVector, Value);
	}
	else
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}


void APlayerCharacter::MoveRight(float Value)
{
	if (Value == 0){ return ;}
	
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void APlayerCharacter::ToggleCrouch()
{
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	
	UCustomMovementComponent* CustomPawnMovementComponent = Cast<UCustomMovementComponent>(PawnMovementComponent);
	if (CustomPawnMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
	{
		return;
	}
	
	if (this->bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void APlayerCharacter::Interact_Implementation()
{
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	
	if (!GetInteractionActor()) {return;}
	
	if (!IInteractInterface::Execute_CanInteractWithActor(GetInteractionActor(), this)) {return;}

	IInteractInterface::Execute_InteractWithActor(GetInteractionActor(), this);
}


void APlayerCharacter::BindASCInput()
{
	ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
	}
	
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EAbilityInputID"), static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel)));

		ASCInputBound = true;
	}

}


void APlayerCharacter::AddStartupEffects()
{
	if (!AbilitySystemComponent.IsValid() || StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
			
		}
	}

	StartupEffectsApplied = true;
}

void APlayerCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UCustomGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	CharacterAbilitiesGiven = true;
}

bool APlayerCharacter::IsAlive()
{
	ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>();
	if (PS)
	{
		return PS->GetAttributeSetBase()->GetHealth() > 0.f;
	}

	return false;
}

AActor* APlayerCharacter::GetInteractionActor()
{
	return InteractionActor;
}

UInventoryComponent* APlayerCharacter::GetInventory()
{
	return InventoryComponent;
}

USkeletalMeshComponent* APlayerCharacter::GetToolComponent()
{
	return ToolComponent;
}

bool APlayerCharacter::GetIsCharacterInteracts()
{
	return IsCharacterInteracts;
}

bool APlayerCharacter::SetIsCharacterInteracts(AActor* ActorToInteract)
{
	if (!ActorToInteract)
	{
		IsCharacterInteracts = false;
		InteractionActor = nullptr;
		return true;
	}

	if (!ActorToInteract->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't set IsCharacterInteracts in %s, because actor to interact is not implement InteractInterface"), *ActorToInteract->GetName());
	}

	IsCharacterInteracts = true;
	InteractionActor = ActorToInteract;
	return true;
}







