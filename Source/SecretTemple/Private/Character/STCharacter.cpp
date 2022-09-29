// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/STMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/InteractInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerState/STPlayerState.h"

// Sets default values
ASTCharacter::ASTCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USTMovementComponent>(ACharacter::CharacterMovementComponentName)),
	InteractionRadius(200.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	//InteractionSphere->SetupAttachment(GetCapsuleComponent());
	//InteractionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//InteractionSphere->SetSphereRadius(InteractionRadius);

	InventoryComponent = CreateDefaultSubobject<USTInventoryComponent>("InventoryComponent");
	
	ToolComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun"));

	
}

// Called when the game starts or when spawned
void ASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);

	//TODO make this via Datatable
	ASTPlayerState* PS = GetPlayerState<ASTPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<USTAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		//We do this to make sure that all the information about the actor will definitely be in the abilities
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
	
	AddStartupEffects();
	
	AddCharacterAbilities();

	//InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASTCharacter::OnBeginOverlapInteractionSphere);
	//InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ASTCharacter::OnEndOverlapInteractionSphere);
}

void ASTCharacter::PostInitializeComponents()
{
	/*
	if (ToolComponent && GetMesh())
	{
		ToolComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("ToolSocket"));
	}
	*/
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	

	USTMovementComponent* CustomPawnMovementComponent = Cast<USTMovementComponent>(PawnMovementComponent);
	if (!CustomPawnMovementComponent) { return; }

	CustomPawnMovementComponent->MaxFlySpeed = CustomPawnMovementComponent->MaxWalkSpeedCrouched;
	CustomPawnMovementComponent->BrakingDecelerationFlying = 2000.f;
	
	Super::PostInitializeComponents();
}

// Called every frame
void ASTCharacter::Tick(float DeltaTime)
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

	//UE_LOG(LogTemp, Warning, TEXT("Hitted actor %s"), *HitResult.GetActor()->GetName())
	InteractionActor = HitResult.GetActor();
	
}

// Called to bind functionality to input
void ASTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASTCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ASTCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn Right / Left", this, &ASTCharacter::Turn);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ASTCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &ASTCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ASTCharacter::Interact);
	
	//TODO clarify
	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void ASTCharacter::LookUp(float Value)
{
	if (IsCharacterInteracts){ return; }
	
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
}

void ASTCharacter::Turn(float Value)
{
	if (IsCharacterInteracts){ return; }
	
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

void ASTCharacter::MoveForward_Implementation(float Value)
{
	if (Value == 0){ return; }
	
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }

	//TODO rewrite this when ladder will be Ability via GameplayAbility
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	
	USTMovementComponent* CustomPawnMovementComponent = Cast<USTMovementComponent>(PawnMovementComponent);
	if (!CustomPawnMovementComponent) { return; }
	
	if (CustomPawnMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::DotProduct(FollowCamera->GetForwardVector(), GetActorUpVector()));
		FVector MovementVector = FVector::DotProduct(FollowCamera->GetForwardVector(), GetActorUpVector()) > -0.5f ? GetActorUpVector() : GetActorUpVector() * -1 + FollowCamera->GetForwardVector();
		MovementVector.Normalize();
		AddMovementInput(MovementVector, Value);
	}
	else
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}


void ASTCharacter::MoveRight(float Value)
{
	if (Value == 0){ return ;}
	
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void ASTCharacter::ToggleCrouch()
{
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	
	UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
	if (!PawnMovementComponent) { return; }
	
	USTMovementComponent* CustomPawnMovementComponent = Cast<USTMovementComponent>(PawnMovementComponent);
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

void ASTCharacter::Interact_Implementation()
{
	if (IsCharacterInteracts){ OnInteractionInterrupted.Broadcast(); }
	
	if (!GetInteractionActor()) {return;}
	
	if (!IInteractInterface::Execute_CanInteract(GetInteractionActor(), this)) {return;}

	IInteractInterface::Execute_Interact(GetInteractionActor(), this);
}


void ASTCharacter::BindASCInput()
{
	ASTPlayerState* PS = GetPlayerState<ASTPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<USTAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	}
	
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("ESTAbilityInputID"), static_cast<int32>(ESTAbilityInputID::Confirm), static_cast<int32>(ESTAbilityInputID::Cancel)));

		ASCInputBound = true;
	}

}


void ASTCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void ASTCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<USTGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

bool ASTCharacter::IsAlive()
{
	ASTPlayerState* PS = GetPlayerState<ASTPlayerState>();
	if (PS)
	{
		return PS->GetAttributeSetBase()->GetHealth() > 0.f;
	}

	return false;
}

AActor* ASTCharacter::GetInteractionActor()
{
	return InteractionActor;
}

USTInventoryComponent* ASTCharacter::GetInventory()
{
	return InventoryComponent;
}

USkeletalMeshComponent* ASTCharacter::GetToolComponent()
{
	return ToolComponent;
}

bool ASTCharacter::GetIsCharacterInteracts()
{
	return IsCharacterInteracts;
}

bool ASTCharacter::SetIsCharacterInteracts(AActor* ActorToInteract)
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

void ASTCharacter::OnBeginOverlapInteractionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap %s"), *OtherActor->GetName())
}

void ASTCharacter::OnEndOverlapInteractionSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap %s"), *OtherActor->GetName())
}






