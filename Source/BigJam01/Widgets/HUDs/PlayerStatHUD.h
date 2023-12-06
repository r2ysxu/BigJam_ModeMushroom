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
	void SetPlayer(class AMainCharacter* Player);
};
