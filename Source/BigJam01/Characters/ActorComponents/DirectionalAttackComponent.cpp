// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionalAttackComponent.h"
#include "../Main/MainCharacter.h"

// Sets default values for this component's properties
UDirectionalAttackComponent::UDirectionalAttackComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UDirectionalAttackComponent::BeginPlay() {
	Super::BeginPlay();
	Owner = Cast<AMainCharacter>(GetOwner()); // Change to generic later
}

void UDirectionalAttackComponent::SetDirectionalMovement(FVector MovementVector) {
	//attack input while midair
	if (FMath::Abs(MovementVector.Z) > 0.f){
		LastMovement = EAttackSwingDirection::VE_J;
	} else if (FMath::Abs(MovementVector.Y) > FMath::Abs(MovementVector.X)) {
		LastMovement = EAttackSwingDirection::VE_F;
	} else {
		LastMovement = EAttackSwingDirection::VE_N;
	}
}

void UDirectionalAttackComponent::InitiateAttack(EAttackType AttackType) {
	UAnimMontage* montage = nullptr;

	if (!bAttackWindowOpen || Owner->GetIsDodging() || !Owner->DrainStamina(StaminaDrainPerAttack)) return;
	if (AttackType == EAttackType::VE_Q) {
		bAttackWindowOpen = false;
		montage = LeftAttackMontages[CurrentAttackIndex];
		CurrentAttackIndex = (CurrentAttackIndex + 1) % LeftAttackMontages.Num();
	} else if (AttackType == EAttackType::VE_E) {
		bAttackWindowOpen = false;
		montage = RightAttackMontages[CurrentAttackIndex];
		CurrentAttackIndex = (CurrentAttackIndex + 1) % RightAttackMontages.Num();
	} else if (AttackType == EAttackType::VE_L) {
		bAttackWindowOpen = false;
		switch (LastMovement) {
			case EAttackSwingDirection::VE_F:
				montage = FrontAttackMontages[CurrentAttackIndex];
				CurrentAttackIndex = (CurrentAttackIndex + 1) % FrontAttackMontages.Num();
				break;
			case EAttackSwingDirection::VE_J:
				montage = JumpAttackMontages[CurrentAttackIndex];
				CurrentAttackIndex = (CurrentAttackIndex + 1) % JumpAttackMontages.Num();
				break;
			case EAttackSwingDirection::VE_N:
				OnAttackStop();
			default: break;
		}
	}
	if (IsValid(montage)) {
		Owner->SetIsAttacking(true);
		Owner->SetDodgeWindow(false);
		ApplyStatusToWeapon(EStatusDebuffType::VE_ZMD);
		float animationDelay = Owner->PlayAnimMontage(montage);
		GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UDirectionalAttackComponent::OnAttackStop, animationDelay, false);
	}
}

void UDirectionalAttackComponent::SetAttackWindow(bool IsOpen) {
	bAttackWindowOpen = IsOpen;
}

void UDirectionalAttackComponent::OnNextCombo() {
	SetAttackWindow(true);
}

void UDirectionalAttackComponent::OnComboReset() {
	SetAttackWindow(false);
	CurrentAttackIndex = 0;
	ClearLastHitTarget();
}

void UDirectionalAttackComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	Owner->SetIsAttacking(false);
	Owner->SetDodgeWindow(true);
}
