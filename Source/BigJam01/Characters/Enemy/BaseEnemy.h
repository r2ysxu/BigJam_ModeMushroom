// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseCharacter.h"
#include "../../Widgets/HUDs/EnemyHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS(Abstract)
class BIGJAM01_API ABaseEnemy : public ABaseCharacter {
	GENERATED_BODY()

private:
	FTimerHandle UpdateHudHander;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float RoamSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDebuffComponent* DebuffComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UWidgetComponent* EnemyHudComponet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UEnemyHUD> EnemyHudClass;
	class UEnemyHUD* EnemyHud;
	
	bool bSleeping = false;
	volatile bool bInAnimLock = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ABaseEnemy();

	virtual bool CheckAlive() override;
	virtual void TakeHitDamage(float Damage);
	virtual float OnHitByOpponent(float Damage, enum EStatusDebuffType Status) override;

	virtual void SetInAnimLock(bool bLock);
	virtual bool GetInAnimLock();

	FORCEINLINE virtual uint8 GetTeam() { return 2; }
	FORCEINLINE bool GetIsSleeping() { return bSleeping; }
	void SetIsSleeping(bool IsSleeping);
	void ShowHud();
	void UpdateHud();
};
