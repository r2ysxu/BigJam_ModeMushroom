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
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(GetOwner())) {
		IsDodging = Owner->GetIsDodging();
	}
}

ACharacter* UMainAnimInstance::GetOwner() {
	return Owner;
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

