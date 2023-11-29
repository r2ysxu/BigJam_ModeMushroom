// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"
#include "../Enemy/BaseEnemy.h"

#include "Engine/LocalPlayer.h"
#include "Components/BoxComponent.h"
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

	MeleeWeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeWeaponBox"));
	MeleeWeaponBox->SetBoxExtent(FVector(5.f, 5.f, 100.f));
	MeleeWeaponBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketR);
	MeleeWeaponBox->bHiddenInGame = false;

	ComboComponet = CreateDefaultSubobject<UComboComponent>(TEXT("ComboComponent"));

}

void AMainCharacter::Tick(float DeltaTime) {
}

UComboComponent* AMainCharacter::GetComboComponent() {
	return ComboComponet;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
	MeleeWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnWeaponMeleeHit);
}

void AMainCharacter::SetDodgeWindow(bool IsOpen) {
	bRollWindowOpen = IsOpen;
}

void AMainCharacter::InitiateAttack(class UAnimMontage* AnimMontage, EAttackType AttackType) {
	if (CanMoveAndAttack()) {
		ComboComponet->InitiateAttack(AnimMontage, AttackType);
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

void AMainCharacter::OnHitByOpponent() {
	if (FlinchMontage) {
		GetMovementComponent()->StopActiveMovement();
		float animationDelay = PlayAnimMontage(FlinchMontage);
		GetWorld()->GetTimerManager().SetTimer(OnFlinchHandler, this, &AMainCharacter::OnFlinchStop, animationDelay, false);
	}
}

void AMainCharacter::AttackL() {
	InitiateAttack(AttackLMontage, EAttackType::VE_L);
}

void AMainCharacter::AttackR() {
	InitiateAttack(AttackRMontage, EAttackType::VE_R);
}

void AMainCharacter::AttackQ() {
	InitiateAttack(AttackQMontage, EAttackType::VE_Q);
}

void AMainCharacter::AttackE() {
	InitiateAttack(AttackEMontage, EAttackType::VE_E);
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

void AMainCharacter::OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || !bAttacking) return;
	ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
	if (IsValid(enemy) && !ComboComponet->IsLastHitEnemy(enemy)) {
		enemy->OnHitByOpponent();
		ComboComponet->MarkLastHitEnemy(enemy);
	}
}
