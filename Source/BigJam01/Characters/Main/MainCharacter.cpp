// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"
#include "../ActorComponents/DirectionalAttackComponent.h"
#include "../ActorComponents/ChargeAttackComponent.h"
#include "../ActorComponents/EnemyReactionComponent.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../../Widgets/HUDs/ComboHUD.h"
#include "../../Widgets/HUDs/PlayerStatHUD.h"
#include "../../BigJam01GameMode.h"

#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
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

	ComboComponet = CreateDefaultSubobject<UComboComponent>(TEXT("ComboAttackComponent"));
	DaoComponet = CreateDefaultSubobject<UDirectionalAttackComponent>(TEXT("DirectionalAttackComponent"));
	ChargeComponent = CreateDefaultSubobject<UChargeAttackComponent>(TEXT("ChargeAttackComponent"));
	EnemyReactionComponent = CreateDefaultSubobject<UEnemyReactionComponent>(TEXT("EnemyReactionComponent"));
}

UComboComponent* AMainCharacter::GetComboComponent() {
	return ComboComponet;
}

UEnemyReactionComponent* AMainCharacter::GetEnemyReactionComponent() {
	return EnemyReactionComponent;
}

void AMainCharacter::SetVolumes(float Master, float Music, float Sfx) {
	MasterVolume = Master;
	BGMVolume = Music;
	SFXVolume = Sfx;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
	SetupHUDs();
	SetupWeapons();
	EquipWeapon(0);
	GetWorld()->GetTimerManager().SetTimer(OnStaminaHandler, this, &AMainCharacter::OnStaminaRegen, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(OnHealthRegenHandler, this, &AMainCharacter::OnHealthRegen, 0.1f, true);
	BGMSereneComponent = UGameplayStatics::CreateSound2D(GetWorld(), BGMSereneSound);
	BGMBattleComponent = UGameplayStatics::CreateSound2D(GetWorld(), BGMBattleSound);
	BGMBattleComponent->Play();
	//GetWorld()->GetTimerManager().SetTimer(OnMusicHandler, this, &AMainCharacter::OnCheckBGMusic, 0.5f, true);
}

void AMainCharacter::SetDodgeWindow(bool IsOpen) {
	bRollWindowOpen = IsOpen;
}

bool AMainCharacter::OnHitTarget(ABaseCharacter* Target, float Damage, EStatusDebuffType Status) {
	if (!bAttacking) return false;
	if (IsValid(Target)) {
		Health = FMath::Min(1.f, Health + Target->OnHitByOpponent(Damage, Status));
		Target->CheckAlive();
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
		PlayerHud->SetVisibility(ESlateVisibility::Hidden);
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

void AMainCharacter::OnHealthRegen() {
	if (!EnemyReactionComponent->HasSubscribers()) {
		Health = FMath::Min(1.f, Health + HealthRegen);
	}
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
		EnemyReactionComponent->NotifyDodge();
	}
}

bool AMainCharacter::CanMove() {
	return !GetIsDodging() && !bFlinching && !bAttacking;
}

bool AMainCharacter::CanMoveAndAttack() {
	return !GetIsDodging() && !bFlinching;
}

void AMainCharacter::SetMovementDirection(FVector MovementVector) {
	DaoComponet->SetDirectionalMovement(MovementVector);
}

void AMainCharacter::OnDodgeRoll() {
	if (bRolling) {
		FVector rollVelocity = GetActorForwardVector() * ROLL_VELOCITY;
		AddMovementInput(rollVelocity);
	}
}

void AMainCharacter::OnDodgeRollStop() {
	GetWorld()->GetTimerManager().ClearTimer(OnDodgeHandler);
	bRolling = false;
	GetMovementComponent()->Velocity = FVector();
}

void AMainCharacter::OnFlinchStop() {
	bFlinching = false;
}

void AMainCharacter::OnCheckBGMusic() {
	if (EnemyReactionComponent->HasSubscribers()) {
		if (BGMSereneComponent) BGMSereneComponent->Stop();
		if (BGMBattleComponent && !BGMBattleComponent->IsPlaying()) BGMBattleComponent->Play();
	} else if (!BGMSereneComponent->IsPlaying()) {
		if (BGMBattleComponent) BGMBattleComponent->Stop();
		if (BGMSereneComponent) BGMSereneComponent->Play();
	}
}

bool AMainCharacter::CheckAlive() {
	if (Health <= 0 && bAlive) {
		bAlive = false;
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetCharacterMovement()->StopMovementImmediately();
		GetMovementComponent()->Deactivate();
		DisableInput(Cast<APlayerController>(GetController()));
		UUserWidget* gameoverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (IsValid(gameoverWidget)) {
			gameoverWidget->AddToViewport();
		}
		GetWorld()->GetTimerManager().SetTimer(OnDeathHandler, this, &AMainCharacter::DeathRestart, 5.f, true);
	}
	return Super::CheckAlive();
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

void AMainCharacter::OnPlayerStart() {
	if (IsValid(PlayerHud)) {
		PlayerHud->SetVisibility(ESlateVisibility::Visible);
	}
}

bool AMainCharacter::HasCharged() {
	return ChargeComponent->OnChargedUp();
}

void AMainCharacter::DeathRestart() {
	ABigJam01GameMode* gamemode = Cast<ABigJam01GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gamemode)) {
		gamemode->RestartGame();
	}
}

bool AMainCharacter::DrainStamina(float Value) {
	if (Value > Stamina) return false;
	else Stamina -= Value;
	return true;
}

float AMainCharacter::OnHitByOpponent(float Damage, EStatusDebuffType Status) {
	Health -= FMath::Min(Health, Damage);
	if (CheckAlive() && FlinchMontage) {
		GetMovementComponent()->StopActiveMovement();
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().SetTimer(OnFlinchHandler, this, &AMainCharacter::OnFlinchStop, animationDelay, false);
	}
	return 0.f;
}

void AMainCharacter::AttackL() {
	if (GetIsAttacking()) return;
	InitiateAttack(EAttackType::VE_L);
}

void AMainCharacter::AttackR() {
	if (GetIsAttacking()) return;
	InitiateAttack(EAttackType::VE_R);
}

void AMainCharacter::AttackQ() {
	if (GetIsAttacking()) return;
	InitiateAttack(EAttackType::VE_Q);
}

void AMainCharacter::AttackE() {
	if (GetIsAttacking()) return;
	InitiateAttack(EAttackType::VE_E);
}

void AMainCharacter::DodgeRoll() {
	if (DodgeMontage && !bRolling && !GetMovementComponent()->IsFalling() && bRollWindowOpen && DrainStamina(DodgeStaminaDrain)) {
		bRolling = true;
		float animationDelay = PlayAnimMontage(DodgeMontage);
		GetWorld()->GetTimerManager().SetTimer(OnDodgeHandler, this, &AMainCharacter::OnDodgeRoll, 0.01f, true, 0);
		GetWorld()->GetTimerManager().SetTimer(OnDodgeStopHandler, this, &AMainCharacter::OnDodgeRollStop, animationDelay, false);
	}
}

void AMainCharacter::EquipWeapon(uint32 WeaponIndex) {
	if (AvailableWeapons.IsValidIndex(WeaponIndex) && !GetIsAttacking()) {
		GetEquippedWeapon()->UnEquip(GetMesh(), EquippableWeaponClasses[EquipedWeaponIndex].SocketName);
		EquipedWeaponIndex = WeaponIndex;
		GetEquippedWeapon()->Equip(GetMesh(), WeaponSocket);

		ComboComponet->SetWeapon(GetEquippedWeapon());
		DaoComponet->SetWeapon(GetEquippedWeapon());
		ChargeComponent->SetWeapon(GetEquippedWeapon());

		DaoComponet->OnAttackStop();
		ComboComponet->OnAttackStop();
		ChargeComponent->OnAttackStop();
	}
}
