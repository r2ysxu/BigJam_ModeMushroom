// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAttackComponent.h"
#include "../Main/MainCharacter.h"

#include "Components/ActorComponent.h"

// Sets default values for this component's properties
UChargeAttackComponent::UChargeAttackComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UChargeAttackComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AMainCharacter>(GetOwner()); // Change to generic later
}

bool UChargeAttackComponent::IsAttackChainable(EAttackType CurrentAttack) {
	return CurrentAttack == EAttackType::VE_R && ComboChain < AttackMontages.Num();
}

void UChargeAttackComponent::InitiateAttack(EAttackType AttackType) {
	if (AttackType != EAttackType::VE_R) return;
	if (IsAttackChainable(AttackType) && !Owner->GetIsDodging() && bAttackWindowOpen && Owner->DrainStamina(StaminaDrainPerAttack)) {
		ApplyStatusToWeapon(EStatusDebuffType::VE_ZMD);
		float animationDelay = Owner->PlayAnimMontage(AttackMontages[ComboChain]);
		GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UChargeAttackComponent::OnAttackStop, animationDelay, false);
		ComboChain++;
	} else {
		ComboChain = 0;
	}
}

void UChargeAttackComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	Owner->SetIsAttacking(false);
	Owner->SetDodgeWindow(true);
}

void UChargeAttackComponent::OnNextCombo() {
	Owner->SetIsAttacking(false);
	SetAttackWindow(false);
}

void UChargeAttackComponent::OnComboReset() {
	SetAttackWindow(false);
	ComboChain = 0;
	Owner->ClearLastHitEnemy();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("ResetCombo"));
}

bool UChargeAttackComponent::OnChargedUp() {
	if (Owner->GetIsCharging()) {
		return true;
	} else {
		ComboChain = 0;
		return false;
	}
}

void UChargeAttackComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}