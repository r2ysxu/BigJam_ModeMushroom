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
	if (bAttacking) return;
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
	if (IsAttackChainable(AttackType) && !Owner->GetIsDodging() && bAttackWindowOpen) {
		bAttacking = true;
		bAttackWindowOpen = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%d"), LastMovement));
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
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("ResetCombo"));
}

void UDirectionalAttackComponent::OnAttackStop() {
	OnComboReset();
	SetAttackWindow(true);
	bAttacking = false;
	Owner->SetIsAttacking(false);
	Owner->SetDodgeWindow(true);
}
