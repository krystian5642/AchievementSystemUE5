#pragma once

#include "Engine/DataTable.h"
#include "AchievementData.generated.h"

UENUM(BlueprintType)
enum class EAchievementType : uint8
{
	Trigger,
	Milestones
};


USTRUCT(BlueprintType)
struct FAchievementData : public FTableRowBase
{
	GENERATED_BODY()

	FAchievementData() 
	{
		AchievementSaveGameSlots.Add(FString("AchievementSaveGameSlot"));
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of the achievement which will be displayed in the achievement UI"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "A detailed desctiption of a specific achievement"))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "An icon which represents an achievement if it's not achieved"))
	UTexture2D* LockedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "An icon which represents an achievement if it's achieved"))
	UTexture2D* AchievedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "The type of the achievement"))
	EAchievementType AchievementType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "All AchievementSaveGameSlots that an achievement will be saved/loaded"))
	TArray<FString> AchievementSaveGameSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "All tags that define what an achievement is about"))
	TArray<FName> AchievementTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Target quantity of the achievement if it's milestiones", EditCondition = "AchievementType == EAchievementType::Milestones"))
	float AchievementTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "if true an achievement target is an integer number", EditCondition = "AchievementType == EAchievementType::Milestones"))
	bool bIntegerTarget;

};