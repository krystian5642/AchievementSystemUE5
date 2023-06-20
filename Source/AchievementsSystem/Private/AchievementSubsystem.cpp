// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSubsystem.h"
#include "AchievementSettings.h"
#include "AchievementData.h"
#include "Kismet/GameplayStatics.h"
#include "AchievementSaveGame.h"
#include "Engine/DataTable.h"
#include "AchievementDetailsHUD.h"

void UAchievementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AchievementSettings = UAchievementSettings::GetSettings();

	if (!AchievementSettings)
	{
		return;
	}

	AchievementDataTable = Cast<UDataTable>(AchievementSettings->AchievementsDataTable.TryLoad());

	InitializeGameFromSavedGame();

	UWorld* World = GetWorld();
	if (World && AchievementSettings->AchievementHUDWidgetClass)
	{
		World->GetTimerManager().SetTimer(CreateAchievementHUDHandle, this, &ThisClass::TryToCreateAchievementHUD, 0.1, true);
	}
}

void UAchievementSubsystem::CompleteAchievement(FName Id)
{
	if (!AchievementSaveGame)
	{
		return;
	}

	FAchievementSaveGameData* AchivSaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(Id);
	if (AchivSaveGameData && !AchivSaveGameData->bIsCompleted)
	{
		AchivSaveGameData->bIsCompleted = true;
		if (AchievementDataTable)
		{
			OnAchievementUnlock.Broadcast(Id,*AchievementDataTable->FindRow<FAchievementData>(Id,FString()));
		}
		UGameplayStatics::SaveGameToSlot(AchievementSaveGame, AchievementSettings->LoadGameFromSaveGameSlot, 0);
	}
}

bool UAchievementSubsystem::DoesAchievementExist(FName Id) const
{
	if (!AchievementDataTable)
	{
		return false;
	}
	return AchievementDataTable->FindRow<FAchievementData>(Id, FString()) != nullptr;
}

float UAchievementSubsystem::GetAchievementProgress(FName Id) const
{
	if (!AchievementSaveGame)
	{
		return 0.0f;
	}

	FAchievementSaveGameData* SaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(Id);
	if (SaveGameData)
	{
		return SaveGameData->CurrentProgress;
	}
	return 0.0f;
}

void UAchievementSubsystem::SetAchievementProgress(FName Id,float NewProgress)
{
	if (!AchievementSaveGame)
	{
		return;
	}

	FAchievementSaveGameData* AchievSaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(Id);
	FAchievementData* AchievementData = AchievementDataTable->FindRow<FAchievementData>(Id, FString());
	if (AchievSaveGameData && AchievementData)
	{
		NewProgress = FMath::Min(NewProgress, AchievementData->AchievementTarget);
		AchievSaveGameData->CurrentProgress = NewProgress;
		OnAchievementProgressUpdate.Broadcast(Id, NewProgress);

		if (GetAchievementPercentProgress(Id) >= 100.0f)
		{
			CompleteAchievement(Id);
		}
		UGameplayStatics::SaveGameToSlot(AchievementSaveGame, AchievementSettings->LoadGameFromSaveGameSlot, 0);
	}
}

void UAchievementSubsystem::AddAchievementProgress(FName Id, float Progress)
{
	float CurrentProgress = GetAchievementProgress(Id);
	SetAchievementProgress(Id, CurrentProgress + Progress);
}

bool UAchievementSubsystem::IsAchievementCompleted(FName Id) const
{
	if (!AchievementSaveGame)
	{
		return 0.0f;
	}

	FAchievementSaveGameData* SaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(Id);
	if (SaveGameData)
	{
		return SaveGameData->bIsCompleted;
	}
	return false;
}

float UAchievementSubsystem::GetAchievementTargetProgress(FName Id) const
{
	if (!AchievementSaveGame || !AchievementDataTable)
	{
		return 0.0f;
	}

	FAchievementData* AchievData = AchievementDataTable->FindRow<FAchievementData>(Id, FString());
	if (AchievData)
	{
		return AchievData->AchievementTarget;
	}
	return 0.0f;
}

bool UAchievementSubsystem::IsAchievementIntTarget(FName Id) const
{
	if (!AchievementSaveGame || !AchievementDataTable)
	{
		return false;
	}

	FAchievementData* AchievData = AchievementDataTable->FindRow<FAchievementData>(Id, FString());
	if (AchievData)
	{
		return AchievData->bIntegerTarget;
	}
	return false;
}

float UAchievementSubsystem::GetAchievementPercentProgress(FName Id) const
{
	if (!AchievementSaveGame || !AchievementDataTable)
	{
		return 0.0f;
	}

	FAchievementSaveGameData* SaveGameData = AchievementSaveGame->AchievementsSaveGameData.Find(Id);
	FAchievementData* AchievData = AchievementDataTable->FindRow<FAchievementData>(Id, FString());
	if (SaveGameData && AchievData)
	{
		return 100.0f * SaveGameData->CurrentProgress / AchievData->AchievementTarget;
	}
	return 0.0f;
}

void UAchievementSubsystem::ResetAchievementProgress(FName Id)
{
	SetAchievementProgress(Id, 0);
}

void UAchievementSubsystem::TryToCreateAchievementHUD()
{
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		AchievementDetailsHUD = CreateWidget<UAchievementDetailsHUD>(Controller,AchievementSettings->AchievementHUDWidgetClass);
		GetWorld()->GetTimerManager().ClearTimer(CreateAchievementHUDHandle);
	}
}

void UAchievementSubsystem::InitializeGameFromSavedGame()
{
	if (UGameplayStatics::DoesSaveGameExist(AchievementSettings->LoadGameFromSaveGameSlot, 0))
	{
		AchievementSaveGame = Cast<UAchievementSaveGame>(UGameplayStatics::LoadGameFromSlot(AchievementSettings->LoadGameFromSaveGameSlot, 0));

	}
	else
	{
		AchievementSaveGame = Cast<UAchievementSaveGame>(UGameplayStatics::CreateSaveGameObject(UAchievementSaveGame::StaticClass()));
	}

	if (AchievementDataTable)
	{
		TArray<FName> AllAchievementsIds = AchievementDataTable->GetRowNames();
		UAchievementSettings* Settings = AchievementSettings;

		AllAchievementsIds.RemoveAll([Settings](const FName& Id) {return !Settings->CanBeModifiedOnSaveGame(Id, Settings->LoadGameFromSaveGameSlot); });

		for (const auto& AchievementId : AllAchievementsIds)
		{
			const bool bWasAchievementSaved = AchievementSaveGame->AchievementsSaveGameData.Contains(AchievementId);
			if (!bWasAchievementSaved)
			{
				AchievementSaveGame->AchievementsSaveGameData.Add(AchievementId, FAchievementSaveGameData());
			}
		}

		TArray<FName> AchievementsToRemove;
		for (const auto& [Id, SaveGameData] : AchievementSaveGame->AchievementsSaveGameData)
		{
			if (!AllAchievementsIds.Contains(Id))
			{
				AchievementsToRemove.Add(Id);
			}
		}

		for (const auto& IdToRemove : AchievementsToRemove)
		{
			AchievementSaveGame->AchievementsSaveGameData.Remove(IdToRemove);
		}
	}
	UGameplayStatics::SaveGameToSlot(AchievementSaveGame, AchievementSettings->LoadGameFromSaveGameSlot, 0);
}
