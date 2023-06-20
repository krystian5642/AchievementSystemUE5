// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AchievementPopup.generated.h"

class UTextBlock;
class UImage;
class UAchievementSubsystem;
class UAchievementSettings;
struct FAchievementData;

UCLASS()
class ACHIEVEMENTSSYSTEM_API UAchievementPopup : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AchievementName;

	UPROPERTY(meta = (BindWidget))
	UImage* AchievementUnlockImage;

	UPROPERTY()
	UAchievementSubsystem* AchievementSubsystem;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnAchivementUnlock(FName AchievementId,const FAchievementData& AchievementData);

protected:
	/** The time that this widget is visible on the viewport, 0 means never removed from viewport */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ForceUnits = "s",ClamMin = 0.0f))
	float TimeOnScreen = 3.0f;

};
