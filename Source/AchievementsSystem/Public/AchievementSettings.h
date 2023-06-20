// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AchievementData.h"
#include "Sound/SoundBase.h"
#include "AchievementSettings.generated.h"

class UDataTable;
class UAchievementPopup;
class UAchievementDetailsHUD;
class UTriggerAchievementEntry;
class UMilestonesAchievementEntry;

UCLASS(config = Game, defaultconfig, Category = "All an achievement related data are set up from here")
class ACHIEVEMENTSSYSTEM_API UAchievementSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** The data table with all achievements that the player can achieve */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup", meta = (AllowedClasses = "DataTable"))
	FSoftObjectPath AchievementsDataTable;

	/** if true a pop up widget will show up on the screen when an achievement has been unlocked */
	UPROPERTY(config,EditAnywhere, Category = "Achievements Setup")
	bool bNotifyAchievementUnlock;

	/** The sound that is played when an achievement has been unlocked */
	UPROPERTY(config,EditAnywhere, Category = "Achievements Setup", meta = (EditCondition = "bNotifyAchievementUnlock", AllowedClasses = "SoundBase"))
	FSoftObjectPath NotifySound;

	/** The slot of the achievement save game from which the game will be loaded */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup")
	FString LoadGameFromSaveGameSlot = FString("AchievementSaveGameSlot");

	/** The subclass of the widget class that stores all achievements progress */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup")
	TSubclassOf<UAchievementDetailsHUD> AchievementHUDWidgetClass;

	/** The subclass of the widget that will popup when an achievement has been unlocked */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup")
	TSubclassOf<UAchievementPopup> AchievementPopupWidgetClass;

	/** The subclass of the widget that will represent a Trigger achievement in the achievement details HUD */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup")
	TSubclassOf<UTriggerAchievementEntry> TriggerAchievementEntry;

	/** The subclass of the widget that will represent a Milestone achievement in the achievement details HUD */
	UPROPERTY(config, EditAnywhere, Category = "Achievements Setup")
	TSubclassOf<UMilestonesAchievementEntry> MilestonesAchievementEntry;

	FAchievementData* GetAchievementData(FName Id) const;
	static UAchievementSettings* GetSettings();

	bool CanBeModifiedOnSaveGame(FName Id, const FString& SaveGameSlot) const;
};