// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AchievementData.h"
#include "AchievementSaveGame.h"
#include "AchievementSubsystem.generated.h"

class UAchievementSettings;
class UDataTable;
class UAchievementDetailsHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementComplete, FName, AchievementId, const FAchievementData&, AchievementData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementProgressUpdate, FName, AchievementId, float, NewProgress);

UCLASS()
class ACHIEVEMENTSSYSTEM_API UAchievementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY()
	UAchievementSettings* AchievementSettings;

	UPROPERTY()
	UAchievementSaveGame* AchievementSaveGame;

	UPROPERTY()
	UDataTable* AchievementDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAchievementDetailsHUD* AchievementDetailsHUD;

	FTimerHandle CreateAchievementHUDHandle;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	void CompleteAchievement(FName Id);

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	bool DoesAchievementExist(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	float GetAchievementProgress(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	void SetAchievementProgress(FName Id, float NewProgress);

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	void AddAchievementProgress(FName Id, float Progress);

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	bool IsAchievementCompleted(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	float GetAchievementTargetProgress(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	bool IsAchievementIntTarget(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	float GetAchievementPercentProgress(FName Id) const;

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	FORCEINLINE void ResetAchievementProgress(FName Id);

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	FORCEINLINE int GetCompletedAchievementCount() const
	{
		return AchievementSaveGame ? AchievementSaveGame->CompletedAchievementsCount : 0;
	}

	UFUNCTION(BlueprintCallable, Category = "AchievementSubsystem")
	FORCEINLINE int GetAchievementCount() const
	{
		return AchievementSaveGame ? AchievementSaveGame->AchievementsSaveGameData.Num() : 0;
	}

	UPROPERTY(BlueprintAssignable, Category = "AchievementSubsystem")
	FOnAchievementProgressUpdate OnAchievementProgressUpdate;

	UPROPERTY(BlueprintAssignable, Category = "AchievementSubsystem")
	FOnAchievementComplete OnAchievementUnlock;

private:
	void TryToCreateAchievementHUD();

	void InitializeGameFromSavedGame();

};
