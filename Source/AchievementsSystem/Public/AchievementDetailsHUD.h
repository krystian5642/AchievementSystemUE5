// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AchievementDetailsHUD.generated.h"

class UScrollBox;
class UVerticalBox;
class UAchievementPopup;
class UAchievementSettings;
class UAchievementSubsystem;

UCLASS()
class ACHIEVEMENTSSYSTEM_API UAchievementDetailsHUD : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* AllAchievements;

	UPROPERTY()
	UAchievementPopup* AchievementPopupWidget;

	UPROPERTY()
	UAchievementSettings* AchievementSettings;

	UPROPERTY()
	UAchievementSubsystem* AchievementSubsystem;

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	void InitializeAchievementsList();

};
