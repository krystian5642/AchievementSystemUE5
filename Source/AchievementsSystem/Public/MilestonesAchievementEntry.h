// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerAchievementEntry.h"
#include "MilestonesAchievementEntry.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ACHIEVEMENTSSYSTEM_API UMilestonesAchievementEntry : public UTriggerAchievementEntry
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UProgressBar* AchievementProgress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentProgress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TargetProgress;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void OnAchievementProgressUpdate(FName AchievementId, float NewProgress);

public:
	virtual void SetAchievementId(FName NewId);
	
};
