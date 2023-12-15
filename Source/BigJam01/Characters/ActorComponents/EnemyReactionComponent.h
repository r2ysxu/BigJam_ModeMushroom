// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyReactionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIGJAM01_API UEnemyReactionComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<class AMeleeEnemy*> SubscribedEnemies;

public:	
	UEnemyReactionComponent();

	void Subscribe(class AMeleeEnemy* Enemy);
	void NotifyDodge();
	void Unsubscribe(class AMeleeEnemy* Enemy);
	bool HasSubscribers();
};
