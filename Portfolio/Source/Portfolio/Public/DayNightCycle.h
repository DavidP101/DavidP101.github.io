// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DayNightCycle.generated.h"

class UCurveFloat;

UCLASS()
class PORTFOLIO_API ADayNightCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycle();

	//Will be called from the questline actor
	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void RestartTimeline() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//These are the curves that will drive colour, intensity and rotation (pitch only) respectively
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Curves")
	class UCurveLinearColor* SunColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Curves")
	UCurveFloat* SunIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Curves")
	UCurveFloat* SunRotation;

	//These are our timeline curves, alpha will drive the above curves while the latter will drive our in-game time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves")
	UCurveFloat* DayToNightAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves")
	UCurveFloat* DayToNightMinutes;

	//Desired day length is in minutes (will get converted to seconds to work with timeline)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Length")
	float DayLengthMinutes;

	//For our sun's directional light and sky light respectively
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Assets")
	class ADirectionalLight* SunLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Assets")
	class ASkyLight* SkyLight;

	//UE4 sky sphere is BP only so we'll just create an event that we can implement in BP
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Light Recapturing")
	void RecaptureSkySphere();

	//How often will we recapture the sky light in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light Recapturing")
	float SkyLightRecaptureTime;

	//prevents GC
	UPROPERTY()
	UTimelineComponent* Timeline;

private:

	//When the timeline is updating it will call this function
	UFUNCTION()
	void TimelineUpdateFloat(const float Value);

	//Delegate for Timeline will call TimelineUpdate()
	FOnTimelineFloat InterpFunc;

	//Game mode
	UPROPERTY()
	class ANewQuestGM* GameMode;

	void StartSkyLightTimer();
	void UpdateGameTime(const float Value);
	void RecaptureSkyLight() const;
	void SetSunRotation(const float Pitch);
	void SetSunColour(const FLinearColor& Colour);
	int32 StartHour;
	int32 CurrentHour;
	int32 CurrentMinute;

	//Will have a seperate timer for recapturing the sky light because of performance issues
	FTimerHandle SkyLightTimerHandle;
};
