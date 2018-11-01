// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NewQuestGM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourlyUpdate, const int32, Hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinuteUpdate, const int32, Hour, const int32, Minute);

/**
 * 
 */
class AQuestline;

UCLASS()
class PORTFOLIO_API ANewQuestGM : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Time Update")
	FOnHourlyUpdate HourlyUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Time Update")
	FOnMinuteUpdate MinuteUpdate;

	UFUNCTION(BlueprintCallable, Category = "Questline Getters")
	AQuestline* GetQuestline() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AQuestline* Questline;
};
