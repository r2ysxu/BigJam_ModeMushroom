// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UBasicAnimInstance : public UAnimInstance {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsFalling;
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual class ACharacter* GetOwner();
	UFUNCTION(BlueprintCallable) FORCEINLINE FVector GetVelocity() { return Velocity; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetGroundSpeed() { return GroundSpeed; }
	UFUNCTION(BlueprintCallable) bool GetShouldMove();
};
