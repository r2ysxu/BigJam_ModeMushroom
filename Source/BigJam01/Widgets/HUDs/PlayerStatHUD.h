// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatHUD.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UPlayerStatHUD : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMainCharacter* Owner;

public:
<<<<<<< HEAD:Source/BigJam01/Widgets/HUDs/PlayerStatHUD.h
	void SetPlayer(class AMainCharacter* Player);
=======
	void SetEnemy(ABaseEnemy* Enemy);
>>>>>>> Added Status effect to enemy HUD:Source/BigJam01/Widgets/HUDs/EnemyHud.h
};
