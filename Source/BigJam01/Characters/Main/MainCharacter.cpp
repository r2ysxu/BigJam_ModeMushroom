// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../Enemy/BaseEnemy.h"

#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABigJam01Character

AMainCharacter::AMainCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ComboComponet = CreateDefaultSubobject<UComboComponent>(TEXT("ComboComponent"));

}

void AMainCharacter::Tick(float DeltaTime) {
}

UComboComponent* AMainCharacter::GetComboComponent() {
	return ComboComponet;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
	SetupWeapons();
	EquipWeapon(0);
}

void AMainCharacter::SetDodgeWindow(bool IsOpen) {
	bRollWindowOpen = IsOpen;
}

bool AMainCharacter::OnHitTarget(ABaseCharacter* Target) {
	if (!bAttacking) return false;
	if (IsValid(Target) && !ComboComponet->IsLastHitEnemy(Target)) {
		Target->OnHitByOpponent();
		ComboComponet->MarkLastHitEnemy(Target);
		return true;
	}
	return false;
}

void AMainCharacter::SetupWeapons() {
	for (int i = 0; i < EquippableWeaponClasses.Num(); i++) {
		FSpawnMeleeWeapon* weaponInfo = &EquippableWeaponClasses[i];
		AMeleeWeapon* weapon = GetWorld()->SpawnActor<AMeleeWeapon>(weaponInfo->WeaponClass);
		if (IsValid(weapon)) {
			weapon->SetWielder(this);
			weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, weaponInfo->SocketName);
			AvailableWeapons.Add(weapon);
		}
	}
}

void AMainCharacter::InitiateAttack(EAttackType AttackType) {
	if (CanMoveAndAttack()) {
		ComboComponet->InitiateAttack(AttackType);
	}
}

bool AMainCharacter::CanMoveAndAttack() {
	return !GetIsDodging() && !bFlinching;
}

void AMainCharacter::OnDodgeRoll() {
	if (bRolling) {
		FVector rollVelocity = GetActorForwardVector() * ROLL_VELOCITY;
		AddMovementInput(rollVelocity);
	}
}

void AMainCharacter::OnDodgeRollStop() {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Dodge Stop"));
	GetWorld()->GetTimerManager().ClearTimer(OnDodgeHandler);
	bRolling = false;
	GetMovementComponent()->Velocity = FVector();
}

void AMainCharacter::OnFlinchStop() {
	bFlinching = false;
}

AMeleeWeapon* AMainCharacter::GetEquippedWeapon() {
	if (!AvailableWeapons.IsValidIndex(EquipedWeaponIndex)) return nullptr;
	else return AvailableWeapons[EquipedWeaponIndex];
}

void AMainCharacter::OnHitByOpponent() {
	if (FlinchMontage) {
		GetMovementComponent()->StopActiveMovement();
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().SetTimer(OnFlinchHandler, this, &AMainCharacter::OnFlinchStop, animationDelay, false);
	}
}

void AMainCharacter::AttackL() {
	InitiateAttack(EAttackType::VE_L);
}

void AMainCharacter::AttackR() {
	InitiateAttack(EAttackType::VE_R);
}

void AMainCharacter::DodgeRoll() {
	if (DodgeMontage && !bRolling && !GetMovementComponent()->IsFalling() && bRollWindowOpen) {
		bRolling = true;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Dodging"));
		float animationDelay = PlayAnimMontage(DodgeMontage);
		GetWorld()->GetTimerManager().SetTimer(OnDodgeHandler, this, &AMainCharacter::OnDodgeRoll, 0.01f, true, 0);
		GetWorld()->GetTimerManager().SetTimer(OnDodgeStopHandler, this, &AMainCharacter::OnDodgeRollStop, animationDelay, false);
	}
}

void AMainCharacter::FocusEnemy() {}

void AMainCharacter::EquipWeapon(uint32 WeaponIndex) {
	if (AvailableWeapons.IsValidIndex(WeaponIndex)) {
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		GetEquippedWeapon()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquippableWeaponClasses[EquipedWeaponIndex].SocketName);
		EquipedWeaponIndex = WeaponIndex;
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		GetEquippedWeapon()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	}
}
