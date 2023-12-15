// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicAnimInstance.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UMainAnimInstance : public UBasicAnimInstance {
	GENERATED_BODY()

private:
	class AMainCharacter* Owner;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsDodging;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual class ABaseCharacter* GetOwner() override;
	UFUNCTION(BlueprintCallable) void NextAttackComboWindow();
	UFUNCTION(BlueprintCallable) void ResetCombo();
	UFUNCTION(BlueprintCallable) void DodgeWindowOpen();
	UFUNCTION(BlueprintCallable) void DodgeWindowClosed();
	UFUNCTION(BlueprintCallable) bool ChargedUp();
};
