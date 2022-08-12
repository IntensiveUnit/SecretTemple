// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/STMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerState/STPlayerState.h"

// Sets default values
ASTCharacter::ASTCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USTMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
}

// Called when the game starts or when spawned
void ASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);


	
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
}


// Called every frame
void ASTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
	//TODO clarify
	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void ASTCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	
}

void ASTCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
	
}

void ASTCharacter::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	
}

void ASTCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	
}

void ASTCharacter::ToggleCrouch()
{
	if (this->bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ASTCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
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







