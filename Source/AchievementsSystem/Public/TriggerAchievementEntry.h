// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TriggerAchievementEntry.generated.h"

class UImage;
class UTextBlock;
class UCheckBox;
class UAchievementSubsystem;
class UAchievementSettings;
class UAchievementSaveGame;
struct FAchievementData;

UCLASS()
class ACHIEVEMENTSSYSTEM_API UTriggerAchievementEntry : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* AchievementImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AchievementName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AchievementDescription;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AchievementCompleted;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void OnAchievementComplete(FName AchievementId, const FAchievementData& AchievementData);

protected:
	UPROPERTY()
	UAchievementSubsystem* AchievementSubsystem;

	UPROPERTY()
	UAchievementSettings* AchievementSettings;

	UPROPERTY()
	UAchievementSaveGame* AchievementSaveGame;

	/** The id of the achievement that this widget represents */
	UPROPERTY(BlueprintSetter = SetAchievementId,BlueprintReadWrite)
	FName AchivId;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetAchievementId(FName NewId);

};
