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
	if (IsAttackChainable(AttackType) && !Owner->GetIsDodging()) {
		if (!bAttackWindowOpen && ComboChain > 0) {
			bAttackWindowMissed = true;
			ClearHUD();
		} else if (!bAttackWindowMissed && Owner->DrainStamina(StaminaDrainPerAttack)) {
			bCanApplyDamage = true;
			bAttackWindowOpen = false;
			Owner->SetIsAttacking(true);
			Owner->SetDodgeWindow(false);
			ApplyStatusToWeapon(ComboNode->Debuff);
			ComboChain++;
			float animationDelay = Owner->PlayAnimMontage(AttackMontages[(uint8)AttackType]);
			GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UComboComponent::OnAttackStop, animationDelay, false);
			if (ComboNode->Debuff == EComboDebuffType::VE_NONE) {
				UpdateHUDs(AttackMontages[(uint8)AttackType], animationDelay);
			} else {
				ClearHUD();
			}
		}
	}
}

void UComboComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	bCanApplyDamage = false;
	Owner->SetIsAttacking(false);
	Owner->SetDodgeWindow(true);
	Owner->ClearLastHitEnemy();
}

void UComboComponent::OnNextCombo() {
	Owner->SetIsAttacking(false);
	SetAttackWindow(true);
}

void UComboComponent::OnComboReset() {
	SetAttackWindow(false);
	ComboChain = 0;
	ComboNode = ComboChains;
	bAttackWindowMissed = false;
	Owner->ClearLastHitEnemy();
	ClearHUD();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("ResetCombo"));
}

void UComboComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}

void UComboComponent::UpdateHUDs(UAnimMontage* AnimMontage, float AnimTime) {
	TArray<const FAnimNotifyEvent*> animNotifies;
	AnimMontage->GetAnimNotifies(0.f, AnimTime, false, animNotifies);
	float startTime = 0;
	float endTime = 0;
	for (int i = 0; i < animNotifies.Num(); i++) {
		if (FName("AnimNotify_NextAttackCombo").IsEqual(animNotifies[i]->GetNotifyEventName())) {
			startTime = animNotifies[i]->GetTriggerTime();
		} else if (FName("AnimNotify_ResetCombo").IsEqual(animNotifies[i]->GetNotifyEventName())) {
			endTime = animNotifies[i]->GetTriggerTime();
		}
		Owner->GetComboHud()->SetTimingRange(AnimTime, FVector2D(startTime, endTime));
		Owner->GetComboHud()->PlayTimer();
	}
}

void UComboComponent::ClearHUD() {
	Owner->GetComboHud()->OnHide();
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
