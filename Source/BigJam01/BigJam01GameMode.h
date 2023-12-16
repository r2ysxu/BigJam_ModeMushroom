// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BigJam01GameMode.generated.h"

UCLASS(minimalapi)
class ABigJam01GameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ABigJam01GameMode();

	void RestartGame();
};



