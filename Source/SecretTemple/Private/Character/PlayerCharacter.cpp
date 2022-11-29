
#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/CustomAttributeSet.h"
#include "Abilities/CustomGameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Components/CustomMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/InventoryComponent.h"
#include "Framework/CustomPlayerState.h"
#include "Interfaces/InteractInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "SecretTemple/SecretTemple.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(CharacterMovementComponentName)),
	InteractionRadius(300.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(GetRootComponent());
	FollowCamera->bUsePawnControlRotation = true;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);
}

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
	const float InteractionSphereRadius = 5.f;
	
	if(!World->SweepSingleByChannel(HitResult, FollowCamera->GetComponentLocation(),
		FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * InteractionRadius,
		FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(InteractionSphereRadius)))
	{
		InteractionActor = nullptr;
		return;
	}

	//We draw a debug sphere for convenience, instead of a crosshair
	const float DebugSphereRadius = 10.f;
	const float DebugSphereSegments = 16.f;
	DrawDebugSphere(GetWorld(),HitResult.Location, DebugSphereRadius, DebugSphereSegments, InteractionActor ? FColor::Green : FColor::Red);
	
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn Right / Left", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	
	// Bind player input to the AbilitySystemComponent
	BindASCInput();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);
	
	AddStartupEffects();
	
	AddCharacterAbilities();
	
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
	MoveForwardInputScalar = Value;
	
	if (Value == 0){ return; }
	
	InterruptCharacterInteraction();
	
	const FVector MovementVector = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));
	
	AddMovementInput(MovementVector, Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value == 0){ return ;}
	
	InterruptCharacterInteraction();
	
	const FVector MovementVector = UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0));
	
	AddMovementInput(MovementVector, Value);
}


void APlayerCharacter::Interact_Implementation()
{
	InterruptCharacterInteraction();
	
	if (!GetInteractionActor()) {return;}
	
	if (!IInteractInterface::Execute_CanInteractWithActor(GetInteractionActor(), this)) {return;}

	IInteractInterface::Execute_InteractWithActor(GetInteractionActor(), this);
}

void APlayerCharacter::BindASCInput()
{
	if (GetAbilitySystemComponent() && IsValid(InputComponent))
	{
		GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EAbilityInputID"), static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel)));
	}
}

void APlayerCharacter::AddStartupEffects()
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), GetAbilitySystemComponent());
		}
	}
	
}

void APlayerCharacter::AddCharacterAbilities()
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}

	for (TSubclassOf<UCustomGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		
		GetAbilitySystemComponent()->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}
	
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent;
	}
	else
	{
		ACustomPlayerState* CustomPlayerState = GetPlayerState<ACustomPlayerState>();
	
		if (!CustomPlayerState) { UE_LOG(LogTemp, Error, TEXT("No CustomPlayerState in character for AbilitySystemComponent")) return nullptr; }

		if (!CustomPlayerState->GetAbilitySystemComponent()) { UE_LOG(LogTemp, Error, TEXT("No AbilitySystemComponent in CustomPlayerState")) return nullptr; }

		AbilitySystemComponent = CustomPlayerState->GetAbilitySystemComponent();
		
		return CustomPlayerState->GetAbilitySystemComponent();
	}
	
}

UCustomAttributeSet* APlayerCharacter::GetAttributeSet()
{
	if (AttributeSet)
	{
		return AttributeSet;
	}
	else
	{
		ACustomPlayerState* CustomPlayerState = GetPlayerStateChecked<ACustomPlayerState>();;
	
		if (!CustomPlayerState) { UE_LOG(LogTemp, Error, TEXT("No CustomPlayerState in character for attributeset")) return nullptr; }
		
		if (!CustomPlayerState->GetAttributeSet()) { UE_LOG(LogTemp, Error, TEXT("No AttrubuteSet in CustomPlayerState")) return nullptr; }

		AttributeSet = CustomPlayerState->GetAttributeSet();
		
		return AttributeSet;
	}
}

float APlayerCharacter::GetMoveForwardInputScalar()
{
	return MoveForwardInputScalar;
}

bool APlayerCharacter::IsAlive()
{
	return GetAttributeSet()->GetHealth() > 0.f;
}

AActor* APlayerCharacter::GetInteractionActor()
{
	return InteractionActor;
}

FVector APlayerCharacter::GetItemDropPosition()
{
	//TODO create checks for obstacles ahead
	const float Offset = 200.f;
	return GetActorLocation() + GetActorForwardVector() * Offset; 
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent()
{
	return InventoryComponent;
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

void APlayerCharacter::InterruptCharacterInteraction() const
{
	if (IsCharacterInteracts)
	{
		if (OnInteractionInterrupted.IsBound())
		{
			OnInteractionInterrupted.Broadcast();
		}
	}
}

float APlayerCharacter::GetHealth()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetHealth();
	}

	return 0.0f;
}

float APlayerCharacter::GetMaxHealth()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetMaxHealth();
	}

	return 0.0f;
}

float APlayerCharacter::GetMana()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetMana();
	}

	return 0.0f;
}

float APlayerCharacter::GetMaxMana()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetMaxMana();
	}

	return 0.0f;
}

float APlayerCharacter::GetStamina()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetStamina();
	}

	return 0.0f;
}

float APlayerCharacter::GetMaxStamina()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetMaxStamina();
	}

	return 0.0f;
}

float APlayerCharacter::GetWalkingSpeed()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetWalkingSpeed();
	}

	return 0.0f;
}

float APlayerCharacter::GetRunningSpeed()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetRunningSpeed();
	}

	return 0.0f;
}

float APlayerCharacter::GetCrouchingSpeed()
{
	if (GetAttributeSet())
	{
		return GetAttributeSet()->GetCrouchingSpeed();
	}

	return 0.0f;
}

