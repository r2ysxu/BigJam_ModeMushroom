// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeEnemy.generated.h"

UENUM(BlueprintType)
enum class EMeleeAttackType : uint8 {
	VE_A1 UMETA(DisplayName = "Attack_1"),
	VE_A2 UMETA(DisplayName = "Attack_2"),
	VE_A3 UMETA(DisplayName = "Attack_3"),
	VE_A4 UMETA(DisplayName = "Attack_4")
};

USTRUCT(BlueprintType)
struct FAttackChain {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<EMeleeAttackType> Attacks;
};

UCLASS()
class BIGJAM01_API AMeleeEnemy : public ACharacter {
	GENERATED_BODY()

private:
	const float ATTACK_DELAY = 2.f;
	const float INITIAL_ATTACK_DELAY = 1.f;

	FTimerHandle InitiateAttackHandler;
	FTimerHandle SingleAttackHandler;
	bool bAttacking = false;
	uint8 SelectedChain = 0;
	uint8 AttackIndex = 0;

	float SingleAttack(class UAnimMontage* AttackMontage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class USphereComponent* MeleeDetectionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> AttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* FlinchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FAttackChain> AttackChains;

	UFUNCTION() void OnWithinMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOutsideMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	virtual void BeginPlay() override;
	void InitiateMeleeAttack();
	void PerformAttack();

	void AttackChain();

public:
	AMeleeEnemy();

	UFUNCTION(BlueprintCallable) bool GetIsAttacking() { return bAttacking; }
};
