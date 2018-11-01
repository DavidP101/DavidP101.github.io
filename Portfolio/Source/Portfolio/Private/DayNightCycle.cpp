// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/DayNightCycle.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveFloat.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "GameFramework/Actor.h"
#include "Components/SkyLightComponent.h"
#include "Components/LightComponent.h"
#include "Public/NewQuestGM.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values
ADayNightCycle::ADayNightCycle()
{
	DayLengthMinutes = 10;
	StartHour = 1;
	CurrentHour = 0;
	CurrentMinute = 0;
	SkyLightRecaptureTime = 5.f;

	//Initialise Timeline
	Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("Timeline"));
	Timeline->SetLooping(true);
	Timeline->SetTimelineLength(1.f);
}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();

	//get the game mode
	GameMode = Cast<ANewQuestGM>(GetWorld()->GetAuthGameMode());

	//bind so that this function gets called whenever we go through timeline
	InterpFunc.BindUFunction(this, FName("TimelineUpdateFloat"));

	//All curves must be valid for us to proceed (do not want to have to keep re-checking in update function) aswell as sunlight/skylight
	if (DayToNightAlpha && SunColour && SunIntensity && SunRotation && DayToNightMinutes && SunLight && SkyLight)
	{
		Timeline->AddInterpFloat(DayToNightAlpha, InterpFunc, FName("Alpha"));
		float DesiredPlayRate = 1 / (DayLengthMinutes * 60);
		Timeline->SetPlayRate(DesiredPlayRate);

		//Start capturing the sky light now on a seperate timer (don't need to update every tick)
		StartSkyLightTimer();

		//Start playing the timeline
		Timeline->Play();
	}
}

//Alpha track of the timeline (will be used to drive all the other curves)
void ADayNightCycle::TimelineUpdateFloat(const float Value)
{
	//Set the sun's rotation (pitch only)
	SetSunRotation(SunRotation->GetFloatValue(Value));

	//Update the current game time calling any events that may be needed
	UpdateGameTime(DayToNightMinutes->GetFloatValue(Value));

	//Set intensity of sun light, skylight
	SunLight->GetLightComponent()->SetIntensity(SunIntensity->GetFloatValue(Value));
	SkyLight->GetLightComponent()->SetIntensity((SunIntensity->GetFloatValue(Value))*0.4);

	//Set colour of sun light
	SetSunColour(SunColour->GetLinearColorValue(Value));

	//Recapture sky sphere
	RecaptureSkySphere();
}

void ADayNightCycle::UpdateGameTime(const float Value)
{
	int32 TempCalculation = ((int32(Value / 60) + StartHour) % 24);

	//Should we update the current hour? e.g. from 0-59 -> 0/60 + 1 % 24 = 1, thus we don't update hour until it's gone to 2.
	if (!(CurrentHour == TempCalculation))
	{
		CurrentHour = TempCalculation;

		if (GameMode)
		{
			GameMode->HourlyUpdate.Broadcast(CurrentHour);
		}
	}

	CurrentMinute = (int32)Value % 60;

	if (GameMode)
	{
		GameMode->MinuteUpdate.Broadcast(CurrentHour, CurrentMinute);
	}
}

void ADayNightCycle::RestartTimeline() const
{
	if (Timeline)
	{
		Timeline->PlayFromStart();
	}
}

void ADayNightCycle::StartSkyLightTimer()
{
	if (SkyLight)
	{
		GetWorldTimerManager().SetTimer(SkyLightTimerHandle, this, &ADayNightCycle::RecaptureSkyLight, SkyLightRecaptureTime, true);
	}
}

void ADayNightCycle::RecaptureSkyLight() const
{
	SkyLight->GetLightComponent()->RecaptureSky();
}

void ADayNightCycle::SetSunRotation(const float Pitch)
{
	SunLight->SetActorRotation(FRotator(Pitch, 30, 0));
}

void ADayNightCycle::SetSunColour(const FLinearColor& Colour)
{
	SunLight->SetLightColor(Colour);
}


