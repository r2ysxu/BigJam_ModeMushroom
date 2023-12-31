// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboComponent.h"
#include "../Main/MainCharacter.h"
#include "../../Widgets/HUDs/ComboHUD.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UComboComponent::UComboComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	ConstructCombos();
	ComboNode = ComboChains;
}

void UComboComponent::InitiateAttack(EAttackType AttackType) {
	if (IsAttackChainable(AttackType) && bAttackWindowOpen && !Owner->GetIsDodging()) {
		if (Owner->DrainStamina(StaminaDrainPerAttack)) {
			UAnimMontage* montage = nullptr;

			switch (AttackType) {
			case EAttackType::VE_L: 
				montage = LMontages[ComboIndex];
				ApplyStatusToWeapon(EStatusDebuffType::VE_POISON);
				break;
			case EAttackType::VE_R:
				montage = RMontages[ComboIndex];
				ApplyStatusToWeapon(EStatusDebuffType::VE_TOXIN);
				break;
			}
			if (IsValid(montage)) {
				bCanApplyDamage = true;
				bAttackWindowOpen = false;
				Owner->SetIsAttacking(true);
				Owner->SetDodgeWindow(false);

				float animationDelay = Owner->PlayAnimMontage(montage);
				GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UComboComponent::OnAttackStop, animationDelay, false);

				ComboIndex = (ComboIndex + 1) % LMontages.Num(); //assume L and R montages always have equal number of anims
			}
		} else {
			ComboNode = ComboChains;
		}
	}
	else {
		ComboIndex = 0;
	}
}

void UComboComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	bCanApplyDamage = false;
	Owner->SetIsAttacking(false);
	Owner->SetDodgeWindow(true);
}

void UComboComponent::OnNextCombo() {
	Owner->SetIsAttacking(false);
	SetAttackWindow(true);
}

void UComboComponent::OnComboReset() {
	SetAttackWindow(false);
	ComboNode = ComboChains;
	ClearLastHitTarget();
}

void UComboComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}

void UComboComponent::ConstructCombos() {
	ComboChains = new FAttackComboNode();

	ComboChains->L = new FAttackComboNode();
	ComboChains->L->L = new FAttackComboNode();

	ComboChains->R = new FAttackComboNode();
	ComboChains->R->R = new FAttackComboNode();
}

// Called when the game starts
void UComboComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AMainCharacter>(GetOwner()); // Change to generic later
}

bool UComboComponent::IsAttackChainable(EAttackType CurrentAttack) {
	if (ComboNode == nullptr) return false;
	else if (CurrentAttack == EAttackType::VE_L && ComboNode->L != nullptr) {
		ComboNode = ComboNode->L;
		return true;
	} else if (CurrentAttack == EAttackType::VE_R && ComboNode->R != nullptr) {
		ComboNode = ComboNode->R;
		return true;
	}
	return false;
}