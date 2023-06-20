// Copyright Epic Games, Inc. All Rights Reserved.

#include "AchievementsSystemGameMode.h"
#include "AchievementsSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAchievementsSystemGameMode::AAchievementsSystemGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
