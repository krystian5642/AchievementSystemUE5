// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerAchievementEntry.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "AchievementData.h"
#include "AchievementSettings.h"
#include "AchievementSubsystem.h"
#include "AchievementSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UTriggerAchievementEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::HitTestInvisible);

	AchievementSettings = UAchievementSettings::GetSettings();
	AchievementSubsystem = GetGameInstance()->GetSubsystem<UAchievementSubsystem>();

	if (!AchievementSettings || !AchievementSubsystem)
	{
		return;
	}

	AchievementSubsystem->OnAchievementUnlock.AddDynamic(this, &ThisClass::OnAchievementComplete);

	if (UGameplayStatics::DoesSaveGameExist(AchievementSettings->LoadGameFromSaveGameSlot,0))
	{
		AchievementSaveGame = Cast<UAchievementSaveGame>(UGameplayStatics::LoadGameFromSlot(AchievementSettings->LoadGameFromSaveGameSlot, 0));
	}
	else
	{
		AchievementSaveGame = Cast<UAchievementSaveGame>(UGameplayStatics::CreateSaveGameObject(UAchievementSaveGame::StaticClass()));
	}
}

void UTriggerAchievementEntry::OnAchievementComplete(FName AchievementId,const FAchievementData& AchievementData)
{
	if (AchivId.IsEqual(AchievementId))
	{
		AchievementImage->SetBrushFromTexture(AchievementData.AchievedIcon);
		AchievementCompleted->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UTriggerAchievementEntry::SetAchievementId(FName NewId)
{
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
			UTexture2D* AchievementTexture = SaveGameData->bIsCompleted ? AchievementData->AchievedIcon : AchievementData->LockedIcon;
			AchievementImage->SetBrushFromTexture(AchievementTexture);

			AchievementName->SetText(FText::FromString(AchievementData->Name));
			AchievementDescription->SetText(FText::FromString(AchievementData->Description));

			ECheckBoxState CheckBoxState = SaveGameData->bIsCompleted ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			AchievementCompleted->SetCheckedState(CheckBoxState);
			AchivId = NewId;
		}
	}
}
