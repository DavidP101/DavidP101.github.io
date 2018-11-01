// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NewQuestGM.h"
#include "Kismet/GameplayStatics.h"
#include "Public/GamePlayerController.h"
#include "Engine/World.h"
#include "Public/Questline.h"

AQuestline* ANewQuestGM::GetQuestline() const
{
	return Questline;
}

void ANewQuestGM::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		TArray<AActor*> Questlines;

		//Get the questline in the world
		UGameplayStatics::GetAllActorsOfClass(World, AQuestline::StaticClass(), Questlines);

		if (Questlines.Num() > 0)
		{
			Questline = Cast<AQuestline>(Questlines.Pop());

			if (AGamePlayerController* PlayerCon = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
			{
				//Create the Player UI
				PlayerCon->CreateInitialUI();

				//Now that we have the UI added we can initialize the quests
				Questline->InitialiseQuests();
			}
		}
	}
}
