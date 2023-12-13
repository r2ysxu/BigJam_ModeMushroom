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

void UDirectionalAttackComponent::SetDirectionalMovement(FVector2D MovementVector) {
	//if (bAttacking) return;
	if (FMath::Abs(MovementVector.Y) > FMath::Abs(MovementVector.X)) {
		LastMovement = EAttackSwingDirection::VE_F;
	} else if (MovementVector.X < 0) {
		LastMovement = EAttackSwingDirection::VE_L;
	} else if (MovementVector.X > 0) {
		LastMovement = EAttackSwingDirection::VE_R;
	} else {
		LastMovement = EAttackSwingDirection::VE_F;
	}
}

bool UDirectionalAttackComponent::IsAttackChainable(EAttackType CurrentAttack) {
	return true;
}

void UDirectionalAttackComponent::InitiateAttack(EAttackType AttackType) {
	if (AttackType != EAttackType::VE_L) return;
	if (IsAttackChainable(AttackType) && !Owner->GetIsDodging() && bAttackWindowOpen && Owner->DrainStamina(StaminaDrainPerAttack)) {
		bAttackWindowOpen = false;
		UAnimMontage* montage;
		switch (LastMovement) {
		case EAttackSwingDirection::VE_F:
			montage = FrontAttackMontages[CurrentAttackIndex];
			CurrentAttackIndex = (CurrentAttackIndex + 1) % FrontAttackMontages.Num();
			break;
		case EAttackSwingDirection::VE_L:
			montage = LeftAttackMontages[CurrentAttackIndex];
			CurrentAttackIndex = (CurrentAttackIndex + 1) % LeftAttackMontages.Num();
			break;
		case EAttackSwingDirection::VE_R:
			montage = RightAttackMontages[CurrentAttackIndex];
			CurrentAttackIndex = (CurrentAttackIndex + 1) % RightAttackMontages.Num();
			break;
		default: montage = nullptr;
		}
		if (IsValid(montage)) {
			Owner->SetIsAttacking(true);
			Owner->SetDodgeWindow(false);
			ApplyStatusToWeapon(EStatusDebuffType::VE_ZMD);
			float animationDelay = Owner->PlayAnimMontage(montage);
			GetWorld()->GetTimerManager().SetTimer(OnAttackHandler, this, &UDirectionalAttackComponent::OnAttackStop, animationDelay, false);
		}
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
