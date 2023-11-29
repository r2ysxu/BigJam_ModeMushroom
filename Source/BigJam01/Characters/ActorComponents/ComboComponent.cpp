// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboComponent.h"
#include "../Main/MainCharacter.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UComboComponent::UComboComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UComboComponent::InitiateAttack(UAnimMontage* AttackMontage, EAttackType AttackType) {
	if (AttackMontage && bAttackWindowOpen && IsAttackChainable(AttackType) && !Owner->GetIsDodging()) {
		bCanApplyDamage = true;
		bAttackWindowOpen = false;
		Owner->SetDodgeWindow(false);
		PreviousAttacks.Push(AttackType);
		float animationDelay = Owner->PlayAnimMontage(AttackMontage);
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
	PreviousAttacks.Empty();
	LastHitEnemy = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("ResetCombo"));
}

void UComboComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}

void UComboComponent::MarkLastHitEnemy(ABaseEnemy* Enemy) {
	LastHitEnemy = Enemy;
}

bool UComboComponent::IsLastHitEnemy(ABaseEnemy* Enemy) {
	return LastHitEnemy == Enemy;
}

// Called when the game starts
void UComboComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AMainCharacter>(GetOwner()); // Change to generic later
}

bool UComboComponent::IsAttackChainable(EAttackType CurrentAttack) {
	if (PreviousAttacks.IsEmpty()) return true;
	else if (PreviousAttacks.Num() > MAX_CHAIN_COMBO) return false;
	EAttackType PreviousAttack = PreviousAttacks.Last();
	return PreviousAttack != CurrentAttack;
}

void UComboComponent::ApplyStatus() {}

