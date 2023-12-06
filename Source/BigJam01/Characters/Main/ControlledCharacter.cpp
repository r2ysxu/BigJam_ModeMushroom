// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlledCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
AControlledCharacter::AControlledCharacter() {

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void AControlledCharacter::BeginPlay() {
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AControlledCharacter::Jump() {
	if (CanMove()) {
		Super::Jump();
	}
}

void AControlledCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AControlledCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AControlledCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AControlledCharacter::Look);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AControlledCharacter::OnScrollAxis);
		// Attack
		EnhancedInputComponent->BindAction(AttackLAction, ETriggerEvent::Triggered, this, &AControlledCharacter::OnAttackLClick);
		EnhancedInputComponent->BindAction(AttackLAction, ETriggerEvent::Completed, this, &AControlledCharacter::OnAttackLReleased);
		EnhancedInputComponent->BindAction(AttackRAction, ETriggerEvent::Triggered, this, &AControlledCharacter::OnAttackRClicked);
		EnhancedInputComponent->BindAction(AttackRAction, ETriggerEvent::Completed, this, &AControlledCharacter::OnAttackRReleased);
		EnhancedInputComponent->BindAction(AttackQAction, ETriggerEvent::Completed, this, &AControlledCharacter::ChangePreviousWeapon);
		EnhancedInputComponent->BindAction(AttackEAction, ETriggerEvent::Completed, this, &AControlledCharacter::ChangeNextWeapon);
		//Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AControlledCharacter::DodgeRoll);
		//Lock on
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &AControlledCharacter::FocusEnemy);
	}
}

void AControlledCharacter::Move(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CanMove()) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		SetMovementDirection(MovementVector);
	}
}

void AControlledCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AControlledCharacter::OnScrollAxis(const FInputActionValue& Value) {
	CameraBoom->TargetArmLength = FMath::Min(400.f, FMath::Max(50, CameraBoom->TargetArmLength - (Value.Get<float>() * CAMERA_SCROLL_SPEED)));
}

void AControlledCharacter::OnAttackLClick() {
	if (!bAttackLHeld) {
		bAttackLHeld = true;
		AttackL();
	}
}

void AControlledCharacter::OnAttackLReleased() {
	bAttackLHeld = false;
}

void AControlledCharacter::OnAttackRClicked() {
	if (!bAttackRHeld) {
		bAttackRHeld = true;
		AttackR();
	}
}

void AControlledCharacter::OnAttackRReleased() {
	bAttackRHeld = false;
}

void AControlledCharacter::ChangeNextWeapon() {
	EquipWeapon((EquipedWeaponIndex + 1) % AvailableWeapons.Num());
}

void AControlledCharacter::ChangePreviousWeapon() {
	EquipWeapon((EquipedWeaponIndex - 1) % AvailableWeapons.Num());
}
