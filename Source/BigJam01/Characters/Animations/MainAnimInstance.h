// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UMainAnimInstance : public UAnimInstance {
	GENERATED_BODY()

private:
	class AMainCharacter* Owner;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsDodging;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable) FORCEINLINE FVector GetVelocity() { return Velocity; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetGroundSpeed() { return GroundSpeed; }
	UFUNCTION(BlueprintCallable) bool GetShouldMove();
	UFUNCTION(BlueprintCallable) void NextAttackComboWindow();
	UFUNCTION(BlueprintCallable) void ResetCombo();
	UFUNCTION(BlueprintCallable) void DodgeWindowOpen();
	UFUNCTION(BlueprintCallable) void DodgeWindowClosed();
};
