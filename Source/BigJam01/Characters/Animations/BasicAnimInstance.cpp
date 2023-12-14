// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimInstance.h"
#include "../../Characters/BaseCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeInitializeAnimation() {
	SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
}

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (IsValid(GetOwner())) {
		Velocity = GetOwner()->GetMovementComponent()->Velocity;
		IsFalling = GetOwner()->GetMovementComponent()->IsFalling();
		GroundSpeed = static_cast<float>(Velocity.Length());
		IsDead = !GetOwner()->GetIsAlive();
	}
}

ABaseCharacter* UBasicAnimInstance::GetOwner() {
	return Cast<ABaseCharacter>(TryGetPawnOwner());
}

bool UBasicAnimInstance::GetShouldMove() {
	if (IsValid(GetOwner())) {
		return GetOwner()->GetMovementComponent()->IsMovingOnGround() && GetOwner()->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0;
	}
	return false;
}
