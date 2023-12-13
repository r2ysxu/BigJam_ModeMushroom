// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseWeapon.h"
#include "../../Characters/ActorComponents/ComboComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

USTRUCT(BlueprintType)
struct FSpawnMeleeWeapon {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMeleeWeapon> WeaponClass;
	UPROPERTY(EditAnywhere)
	FName SocketName;
};

UCLASS()
class BIGJAM01_API AMeleeWeapon : public ABaseWeapon {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* WeaponMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MeleeWeaponBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BaseDamage = 0.1f;
	class ABaseEnemy* LastHitEnemy = nullptr;

	EStatusDebuffType CurrentEffect;

	virtual void BeginPlay() override;

	UFUNCTION()	void OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	AMeleeWeapon();

	void ApplyDebuffEnhancement(EStatusDebuffType DebuffType);
};
