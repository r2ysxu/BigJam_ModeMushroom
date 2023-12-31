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
	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* WeaponFXSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraComponent* WeaponFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* PoisonSplatterFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* ToxinSplatterFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MeleeWeaponBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BaseDamage = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double WeaponLength;
	class ABaseCharacter* LastHitEnemy = nullptr;
	bool bEquipped = false;

	EStatusDebuffType CurrentEffect;

	virtual void BeginPlay() override;

	UFUNCTION()	void OnWeaponMeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	AMeleeWeapon();

	void ApplyDebuffEnhancement(EStatusDebuffType DebuffType);
	void MarkLastHitEnemy(class ABaseCharacter* Enemy);
	void ClearLastHitEnemy();
	void Equip(class USceneComponent* Mesh, FName SocketName);
	void UnEquip(class USceneComponent* Mesh, FName SocketName);
};
