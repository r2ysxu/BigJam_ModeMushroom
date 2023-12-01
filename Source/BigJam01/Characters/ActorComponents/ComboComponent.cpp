// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboComponent.h"
#include "../Main/MainCharacter.h"
#include "../../Weapons/Melee/MeleeWeapon.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UComboComponent::UComboComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	ConstructCombos();
	ComboNode = ComboChains;
	LastHitEnemy = nullptr;
}

void UComboComponent::InitiateAttack(EAttackType AttackType) {
	if (bAttackWindowOpen && IsAttackChainable(AttackType) && !Owner->GetIsDodging()) {
		bCanApplyDamage = true;
		bAttackWindowOpen = false;
		Owner->SetDodgeWindow(false);
		ApplyStatusToWeapon();
		float animationDelay = Owner->PlayAnimMontage(AttackMontages[(uint8) AttackType]);
		GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UComboComponent::OnAttackStop, animationDelay, false);
	}
}

void UComboComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	bCanApplyDamage = false;
	Owner->SetDodgeWindow(true);
}

void UComboComponent::OnNextCombo() {
	SetAttackWindow(true);
}

void UComboComponent::OnComboReset() {
	SetAttackWindow(false);
	ComboNode = ComboChains;
	LastHitEnemy = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("ResetCombo"));
}

void UComboComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}

void UComboComponent::MarkLastHitEnemy(ABaseCharacter* Enemy) {
	LastHitEnemy = Enemy;
}

bool UComboComponent::IsLastHitEnemy(ABaseCharacter* Enemy) {
	return LastHitEnemy == Enemy;
}

void UComboComponent::ConstructCombos() {
	ComboChains = new FAttackComboNode();
	ComboChains->L = new FAttackComboNode();
	ComboChains->L->L = new FAttackComboNode();
	ComboChains->L->L->L = new FAttackComboNode(EComboDebuffType::VE_BURST);
	ComboChains->L->L->R = new FAttackComboNode(EComboDebuffType::VE_SUNDER);

	ComboChains->R = new FAttackComboNode();
	ComboChains->R->R = new FAttackComboNode();
	ComboChains->R->R->R = new FAttackComboNode(EComboDebuffType::VE_POISON);
	ComboChains->R->R->L = new FAttackComboNode(EComboDebuffType::VE_SLEEP);
}

// Called when the game starts
void UComboComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AMainCharacter>(GetOwner()); // Change to generic later
}

bool UComboComponent::IsAttackChainable(EAttackType CurrentAttack) {
	if (ComboNode == nullptr) return false;
	else if (CurrentAttack == EAttackType::VE_L && ComboNode->L) {
		ComboNode = ComboNode->L;
		return true;
	} else if (CurrentAttack == EAttackType::VE_R && ComboNode->R) {
		ComboNode = ComboNode->R;
		return true;
	}
	return false;
}

void UComboComponent::SetWeapon(AMeleeWeapon* EquippedWeapon) {
	Weapon = EquippedWeapon;
}

void UComboComponent::ApplyStatusToWeapon() {
	if (IsValid(Weapon)) {
		Weapon->ApplyDebuffEnhancement(ComboNode->Debuff);
	}
}
