// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComboHUD.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UComboHUD : public UUserWidget {
	GENERATED_BODY()

private:
	const float TIME_INCREMENT = 0.01f;

	FTimerHandle TimingHandle;

	void IncrementTime();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimingProgress;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimingStartP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimingEndP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimingTotal;

public:
	
	void PlayTimer();
	void SetTimingRange(float TotalTime, FVector2D Range);
	UFUNCTION(BlueprintImplementableEvent) void StopHideTransition();
	UFUNCTION(BlueprintImplementableEvent) void OnHide();
};
