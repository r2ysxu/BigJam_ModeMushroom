// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseEnemy.h"

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
class BIGJAM01_API AMeleeEnemy : public ABaseEnemy {
	GENERATED_BODY()

private:
	const float ATTACK_DELAY = .5f;
	const FName WeaponSocket = FName("weapon");

	class AMeleeEnemyController* AiController;

	FTimerHandle InitiateAttackHandler;
	FTimerHandle SingleAttackHandler;
	FTimerHandle FlinchHandler;
	FTimerHandle DashHandler;
	uint8 SelectedChain = 0;
	uint8 AttackIndex = 0;

	volatile bool bAttackSwing = false;
	volatile bool bFlinching = false;
	volatile bool bDashing = false;

	void SingleAttack(EMeleeAttackType AttackType);
	void ClearAttacks();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class USphereComponent* MeleeDetectionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> AttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* FlinchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DashFwdMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DashBackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FAttackChain> AttackChains;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UBoxComponent* MeleeWeaponBox;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Attack")
	float RecoveryDelay = 1.f;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Attack")
	float DodgeChance = 0.5f;

	UFUNCTION() void OnWithinMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOutsideMeleeRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	UFUNCTION() void OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;
	void InitiateMeleeAttack();
	void PerformAttack();
	void AttackChain();
	void OnFlinchRecover();
	void OnDashStop();

public:
	AMeleeEnemy();

	void SetAiController(class AMeleeEnemyController* MEController);
	virtual bool CheckAlive() override;
	UFUNCTION(BlueprintCallable) bool GetIsAttacking() { return bAttacking; }
	UFUNCTION(BlueprintCallable) void NextMoveLocation(FVector Path);
	virtual float OnHitByOpponent(float Damage, EStatusDebuffType Status) override;
	void DashForward();
	void DashBack();
	bool GetIsDashing();
};
