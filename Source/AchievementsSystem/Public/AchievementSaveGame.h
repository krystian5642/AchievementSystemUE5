// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AchievementSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FAchievementSaveGameData
{
	GENERATED_BODY()

	FAchievementSaveGameData() 
		: bIsCompleted(false)
		, CurrentProgress(0.0f) 
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentProgress;
};

UCLASS()
class ACHIEVEMENTSSYSTEM_API UAchievementSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UAchievementSaveGame()
		: CompletedAchievementsCount(0) 
	{}

	/** An achievement save game data is mapped by the unique achievemnt Id */
	UPROPERTY()
	TMap<FName, FAchievementSaveGameData> AchievementsSaveGameData;

	UPROPERTY()
	int CompletedAchievementsCount;
};
