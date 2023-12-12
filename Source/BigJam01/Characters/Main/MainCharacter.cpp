// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"
#include "../ActorComponents/DirectionalAttackComponent.h"
#include "../ActorComponents/ChargeAttackComponent.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../Enemy/BaseEnemy.h"
#include "../../Widgets/HUDs/ComboHUD.h"
#include "../../Widgets/HUDs/PlayerStatHUD.h"

#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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

	ComboComponet = CreateDefaultSubobject<UComboComponent>(TEXT("ComboAttackComponent"));
	DaoComponet = CreateDefaultSubobject<UDirectionalAttackComponent>(TEXT("DirectionalAttackComponent"));
	ChargeComponent = CreateDefaultSubobject<UChargeAttackComponent>(TEXT("ChargeAttackComponent"));
}

void AMainCharacter::Tick(float DeltaTime) {
	if (bFocusing && FocusedTarget) {
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FocusedTarget->GetActorLocation());
		GetController()->SetControlRotation(FMath::RInterpTo(GetActorRotation(), rotation, DeltaTime, 2.f));
	}
}

UComboComponent* AMainCharacter::GetComboComponent() {
	return ComboComponet;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
	SetupHUDs();
	SetupWeapons();
	EquipWeapon(0);
	GetWorld()->GetTimerManager().SetTimer(OnStaminaHandler, this, &AMainCharacter::OnStaminaRegen, 0.5f, true);
}

void AMainCharacter::SetDodgeWindow(bool IsOpen) {
	bRollWindowOpen = IsOpen;
}

bool AMainCharacter::OnHitTarget(ABaseCharacter* Target, float Damage, EStatusDebuffType Status) {
	if (!bAttacking) return false;
	if (IsValid(Target) && !IsLastHitEnemy(Target)) {
		Health = FMath::Min(1.f, Health + Target->OnHitByOpponent(Damage, Status));
		Target->CheckAlive();
		MarkLastHitEnemy(Target);
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

void AMainCharacter::SetupHUDs() {
	PlayerHud = CreateWidget<UPlayerStatHUD>(GetWorld(), PlayerHudClass);
	if (PlayerHud) {
		PlayerHud->SetPlayer(this);
		PlayerHud->AddToViewport();
	}
}

void AMainCharacter::OnNextCombo() {
	switch (EquipedWeaponIndex) {
	case 0: ComboComponet->OnNextCombo(); break;
	case 1: DaoComponet->OnNextCombo(); break;
	}
}

void AMainCharacter::OnComboReset() {
	switch (EquipedWeaponIndex) {
	case 0: ComboComponet->OnComboReset(); break;
	case 1: DaoComponet->OnComboReset(); break;
	}
}

void AMainCharacter::OnStaminaRegen() {
	if (!bAttacking && !GetIsDodging()) {
		Stamina = FMath::Min(1.f, StaminaRegenRate + Stamina);
	}
}

void AMainCharacter::MarkLastHitEnemy(ABaseCharacter* Enemy) {
	LastHitEnemy = Enemy;
}

void AMainCharacter::ClearLastHitEnemy() {
	LastHitEnemy = nullptr;
}

bool AMainCharacter::IsLastHitEnemy(ABaseCharacter* Enemy) {
	return LastHitEnemy == Enemy;
}

void AMainCharacter::InitiateAttack(EAttackType AttackType) {
	if (CanMoveAndAttack()) {
		switch (EquipedWeaponIndex) {
		case 0: ComboComponet->InitiateAttack(AttackType); break;
		case 1: 
			DaoComponet->InitiateAttack(AttackType);
			ChargeComponent->InitiateAttack(AttackType);
			break;
		}
	}
}

bool AMainCharacter::CanMove() {
	return !GetIsDodging() && !bFlinching && !bAttacking;
}

bool AMainCharacter::CanMoveAndAttack() {
	return !GetIsDodging() && !bFlinching;
}

void AMainCharacter::SetMovementDirection(FVector2D MovementVector) {
	DaoComponet->SetDirectionalMovement(MovementVector);
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

void AMainCharacter::SetIsAttacking(bool IsAttacking) {
	bAttacking = IsAttacking;
}

AMeleeWeapon* AMainCharacter::GetEquippedWeapon() {
	if (!AvailableWeapons.IsValidIndex(EquipedWeaponIndex)) return nullptr;
	else return AvailableWeapons[EquipedWeaponIndex];
}

float AMainCharacter::GetStamina() {
	return Stamina;
}

bool AMainCharacter::HasCharged() {
	return ChargeComponent->OnChargedUp();
}

bool AMainCharacter::DrainStamina(float Value) {
	if (Value > Stamina) return false;
	else Stamina -= Value;
	return true;
}

float AMainCharacter::OnHitByOpponent(float Damage, EStatusDebuffType Status) {
	if (FlinchMontage) {
		GetMovementComponent()->StopActiveMovement();
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().SetTimer(OnFlinchHandler, this, &AMainCharacter::OnFlinchStop, animationDelay, false);
		Health -= Damage;
	}
	return 0.f;
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

void AMainCharacter::FocusEnemy() {
	float Range = 500.f;
	const TArray<AActor*> ignored;
	TArray<FHitResult> result;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() * 5.f, 360.f, UEngineTypes::ConvertToTraceType(ECC_Pawn), false, ignored, EDrawDebugTrace::ForDuration, result, true);
	for (size_t i = 0; i < result.Num(); i++) {
		ABaseEnemy* target = Cast<ABaseEnemy>(result[i].GetActor());
		if (IsValid(target)) {
			FocusedTarget = target;
			bFocusing = !bFocusing;
			bUseControllerRotationYaw = bFocusing;
			GetCharacterMovement()->bOrientRotationToMovement = !bFocusing;
			break;
		}
	}
}

void AMainCharacter::EquipWeapon(uint32 WeaponIndex) {
	if (AvailableWeapons.IsValidIndex(WeaponIndex)) {
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		GetEquippedWeapon()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquippableWeaponClasses[EquipedWeaponIndex].SocketName);
		EquipedWeaponIndex = WeaponIndex;
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		GetEquippedWeapon()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
		ComboComponet->SetWeapon(GetEquippedWeapon());
	}
}
