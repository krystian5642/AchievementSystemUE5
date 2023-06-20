// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSettings.h"

FAchievementData* UAchievementSettings::GetAchievementData(FName Id) const
{
	UDataTable* const AchieveDataTableObj = Cast<UDataTable>(AchievementsDataTable.TryLoad());
	if (AchieveDataTableObj)
	{
		return AchieveDataTableObj->FindRow<FAchievementData>(Id, FString());
	}
	return nullptr;
}

UAchievementSettings* UAchievementSettings::GetSettings()
{
	return GetMutableDefault<UAchievementSettings>();
}

bool UAchievementSettings::CanBeModifiedOnSaveGame(FName Id, const FString& SaveGameSlot) const
{
	FAchievementData* AchivData = GetAchievementData(Id);
	if (AchivData)
	{
		return AchivData->AchievementSaveGameSlots.FindByPredicate([SaveGameSlot](const FString& GameSlot) {return GameSlot == SaveGameSlot; }) != nullptr;
	}
	return false;
}
