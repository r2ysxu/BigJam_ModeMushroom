// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "../Main/MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	Owner = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (IsValid(Owner)) {
		Velocity = Owner->GetMovementComponent()->Velocity;
		IsFalling = Owner->GetMovementComponent()->IsFalling();
		GroundSpeed = static_cast<float>(Velocity.Length());
		IsDodging = Owner->GetIsDodging();
	}
}

bool UMainAnimInstance::GetShouldMove() {
	if (IsValid(Owner)) {
		return Owner->GetMovementComponent()->IsMovingOnGround() && Owner->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0;
	}
	return false;
}

void UMainAnimInstance::NextAttackComboWindow() {
	Owner->GetComboComponent()->OnNextCombo();
}

void UMainAnimInstance::ResetCombo() {
	Owner->GetComboComponent()->OnComboReset();
}

void UMainAnimInstance::DodgeWindowOpen() {
	Owner->SetDodgeWindow(true);
}

void UMainAnimInstance::DodgeWindowClosed() {
	Owner->SetDodgeWindow(false);
}

