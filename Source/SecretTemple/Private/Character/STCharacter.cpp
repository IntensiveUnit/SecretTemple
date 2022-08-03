// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASTCharacter::ASTCharacter()
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

	GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);\
	
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
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTCharacter::Jump);
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



