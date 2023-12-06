// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatHUD.h"
#include "../../Characters/Main/MainCharacter.h"

void UPlayerStatHUD::SetPlayer(AMainCharacter* Player) {
	Owner = Player;
}