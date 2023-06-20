// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementDetailsHUD.h"
#include "AchievementPopup.h"
#include "AchievementSettings.h"
#include "AchievementSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TriggerAchievementEntry.h"
#include "Components/VerticalBox.h"
#include "AchievementData.h"
#include "MilestonesAchievementEntry.h"

void UAchievementDetailsHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	AchievementSettings = UAchievementSettings::GetSettings();
	AchievementSubsystem = GetGameInstance()->GetSubsystem<UAchievementSubsystem>();

	if (AchievementSettings && AchievementSettings->bNotifyAchievementUnlock && AchievementSettings->AchievementPopupWidgetClass)
	{
		AchievementPopupWidget = CreateWidget<UAchievementPopup>(UGameplayStatics::GetPlayerController(this,0),
			AchievementSettings->AchievementPopupWidgetClass);
	}

	InitializeAchievementsList();
}

void UAchievementDetailsHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeGameAndUI InputMode;
		bIsFocusable = true;
		InputMode.SetWidgetToFocus(TakeWidget());
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UAchievementDetailsHUD::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}

}

void UAchievementDetailsHUD::InitializeAchievementsList()
{
	if (!AchievementSettings || !AchievementSettings->TriggerAchievementEntry || !AchievementSettings->MilestonesAchievementEntry)
	{
		return;
	}
	UDataTable* AchivementDataTable = Cast<UDataTable>(AchievementSettings->AchievementsDataTable.TryLoad());

	if (!AchivementDataTable)
	{
		return;
	}
	const TMap<FName, uint8*>& AllData = AchivementDataTable->GetRowMap();
	TMap<FName, FAchievementData> AllAchievementData;

	for (const auto& [Id, Data] : AllData)
	{
		if (AchievementSettings->CanBeModifiedOnSaveGame(Id, AchievementSettings->LoadGameFromSaveGameSlot))
		{
			AllAchievementData.Add(Id, *reinterpret_cast<FAchievementData*>(Data));
		}	
	}

	for (const auto& [Id, AchievementData] : AllAchievementData)
	{
		switch (AchievementData.AchievementType)
		{
		case EAchievementType::Trigger:
			{
				UTriggerAchievementEntry* EntryWidget = CreateWidget<UTriggerAchievementEntry>
					(
						UGameplayStatics::GetPlayerController(this, 0),
						AchievementSettings->TriggerAchievementEntry
					);
				EntryWidget->SetAchievementId(Id);
				AllAchievements->AddChild(EntryWidget);
				EntryWidget->SetPadding(FMargin(0, 0, 0, 5));
			}
			break;

		case EAchievementType::Milestones:
			{
				UMilestonesAchievementEntry* EntryWidget = CreateWidget<UMilestonesAchievementEntry>
					(
						UGameplayStatics::GetPlayerController(this, 0),
						AchievementSettings->MilestonesAchievementEntry
					);
				EntryWidget->SetAchievementId(Id);
				AllAchievements->AddChild(EntryWidget);
				EntryWidget->SetPadding(FMargin(0, 0, 0, 5));
			}	
			break;

		default:
			break;
		}
	}

}
