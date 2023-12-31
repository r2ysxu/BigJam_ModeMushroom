// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "../Main/MainCharacter.h"
#include "../ActorComponents/ComboComponent.h"
#include "BasicAnimInstance.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	Owner = Cast<AMainCharacter>(TryGetPawnOwner());
	SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(GetOwner())) {
		IsDodging = Owner->GetIsDodging();
	}
}

ABaseCharacter* UMainAnimInstance::GetOwner() {
	return Owner;
}

void UMainAnimInstance::NextAttackComboWindow() {
	Owner->OnNextCombo();
}

void UMainAnimInstance::ResetCombo() {
	Owner->OnComboReset();
}

void UMainAnimInstance::DodgeWindowOpen() {
	Owner->SetDodgeWindow(true);
}

void UMainAnimInstance::DodgeWindowClosed() {
	Owner->SetDodgeWindow(false);
}

bool UMainAnimInstance::ChargedUp() {
	return Owner->HasCharged();
}

