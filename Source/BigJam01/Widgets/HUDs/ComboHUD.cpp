// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboHUD.h"

void UComboHUD::IncrementTime() {
	TimingProgress = FMath::Min(1.f, TimingProgress + TIME_INCREMENT / TimingTotal);
}

void UComboHUD::PlayTimer() {
	StopHideTransition();
	SetVisibility(ESlateVisibility::Visible);
	TimingProgress = 0;
	GetWorld()->GetTimerManager().SetTimer(TimingHandle, this, &UComboHUD::IncrementTime, TIME_INCREMENT, true);
}

void UComboHUD::SetTimingRange(float TotalTime, FVector2D RangeTime) {
	TimingTotal = TotalTime;
	TimingStartP = RangeTime.X / TotalTime;
	TimingEndP  = RangeTime.Y / TotalTime;
}
