// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHUD.h"
#include "../../Characters/Enemy/BaseEnemy.h"

void UEnemyHUD::SetEnemy(ABaseEnemy* Enemy) {
	Owner = Enemy;
}