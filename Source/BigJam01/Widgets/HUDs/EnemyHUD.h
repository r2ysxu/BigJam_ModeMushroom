// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHUD.generated.h"

/**
 *
 */
UCLASS()
class BIGJAM01_API UEnemyHUD : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ABaseEnemy* Owner;

public:
	void SetEnemy(class ABaseEnemy* Enemy);
};
