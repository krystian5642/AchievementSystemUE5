// Fill out your copyright notice in the Description page of Project Settings.


#include "MilestonesAchievementEntry.h"
#include "AchievementSubsystem.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AchievementSettings.h"

void UMilestonesAchievementEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AchievementSubsystem)
	{
		AchievementSubsystem->OnAchievementProgressUpdate.AddDynamic(this,&ThisClass::OnAchievementProgressUpdate);
	}
}

void UMilestonesAchievementEntry::OnAchievementProgressUpdate(FName AchievementId, float NewProgress)
{
	if (AchivId.IsEqual(AchievementId))
	{
		FNumberFormattingOptions Options;

		const bool bIsIntTarget = AchievementSubsystem->IsAchievementIntTarget(AchievementId);
		int32 MaxDigitNumber = bIsIntTarget ? 0 : 1;
		Options.SetMaximumFractionalDigits(MaxDigitNumber);

		const float Progress = AchievementSubsystem->GetAchievementProgress(AchievementId);

		CurrentProgress->SetText(FText::AsNumber(Progress, &Options));

		const float AchievementTarget = AchievementSubsystem->GetAchievementTargetProgress(AchievementId);
		TargetProgress->SetText(FText::AsNumber(AchievementTarget,&Options));

		const float AchievementPercent = Progress / AchievementTarget;
		AchievementProgress->SetPercent(AchievementPercent);
	}
}

void UMilestonesAchievementEntry::SetAchievementId(FName NewId)
{
	Super::SetAchievementId(NewId);

	if (!AchievementSettings || !AchievementSaveGame)
	{
		return;
	}

	FAchievementData* AchievementData = AchievementSettings->GetAchievementData(NewId);
	if (AchievementData)
	{
		FAchievementSaveGameData* SaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(NewId);
		if (SaveGameData)
		{
			OnAchievementProgressUpdate(NewId, SaveGameData->CurrentProgress);
		}
	}
}
