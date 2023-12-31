// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAttackComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirectionalAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackSwingDirection : uint8 {
	VE_F UMETA(DisplayName = "Front"),
	VE_L UMETA(DisplayName = "Left"),
	VE_R UMETA(DisplayName = "Right"),
	VE_J UMETA(DisplayName = "Jump"),
	VE_N UMETA(DisplayName = "None"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UDirectionalAttackComponent : public UBaseAttackComponent {
	GENERATED_BODY()

private:
	int CurrentAttackIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaDrainPerAttack = .1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> FrontAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> LeftAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> RightAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> JumpAttackMontages;

	EAttackSwingDirection LastMovement = EAttackSwingDirection::VE_F;
	volatile bool bAttackWindowOpen = true;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UDirectionalAttackComponent();

	void SetDirectionalMovement(FVector MovementVector);
	void InitiateAttack(EAttackType AttackType);
	void SetAttackWindow(bool IsOpen);
	void OnNextCombo();
	void OnComboReset();
	void OnAttackStop();
};
