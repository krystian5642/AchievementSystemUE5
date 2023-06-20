// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementPopup.h"
#include "AchievementSubsystem.h"
#include "Components/TextBlock.h"
#include "AchievementData.h"
#include "Components/Image.h"
#include "AchievementSettings.h"

void UAchievementPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AchievementSubsystem = GetGameInstance()->GetSubsystem<UAchievementSubsystem>();

	if (AchievementSubsystem)
	{
		AchievementSubsystem->OnAchievementUnlock.AddDynamic(this, &ThisClass::OnAchivementUnlock);
	}
}

void UAchievementPopup::OnAchivementUnlock(FName AchievementId,const FAchievementData& AchievementData)
{
	AchievementName->SetText(FText::FromString(AchievementData.Name));
	AchievementUnlockImage->SetBrushFromTexture(AchievementData.AchievedIcon);

	SetAnchorsInViewport(FAnchors(1, 1));
	SetAlignmentInViewport(FVector2D(1.1, 1.1));
	AddToViewport();

	if (UWorld* World = GetWorld())
	{
		FTimerHandle RemoveFromVieportHandle;
		World->GetTimerManager().SetTimer(RemoveFromVieportHandle, this, &ThisClass::RemoveFromParent, TimeOnScreen, false);
	}
}
