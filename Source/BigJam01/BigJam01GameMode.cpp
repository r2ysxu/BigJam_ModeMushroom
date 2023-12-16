// Copyright Epic Games, Inc. All Rights Reserved.

#include "BigJam01GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ABigJam01GameMode::ABigJam01GameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Main/Characters/MainCharacter/MainCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABigJam01GameMode::RestartGame() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}
