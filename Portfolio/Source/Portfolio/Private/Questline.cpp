// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Questline.h"
#include "Engine/World.h"
#include "Public/GamePlayerController.h"
#include "Public/GameHUD.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AQuestline::AQuestline()
{

}

AActor* AQuestline::GetObjectiveActor(const int32 QuestIndex)
{
	AActor* QuestActor = nullptr;

	if (ActiveQuests.IsValidIndex(QuestIndex)) //small sanity check to make sure it's actually valid
	{
		for (FObjective Objective : ActiveQuests[QuestIndex].Objectives)
		{
			if (ActiveQuests[QuestIndex].Stage == Objective.Stage) //if the stage is equal to the objective stage
			{
				if (!Objective.ObjectiveID.IsEmpty()) //if the objective id is empty then don't bother trying to find it
				{
					if (QuestActorRefs.Find(Objective.ObjectiveID))
					{
						QuestActor = *QuestActorRefs.Find(Objective.ObjectiveID);
					}
				}
			}
		}
	}

	return QuestActor;
}

// Called when the game starts or when spawned
void AQuestline::BeginPlay()
{
	Super::BeginPlay();

	//Decided to call init quests in player controller instead since it makes calls to the UI
}

void AQuestline::ActivateQuests(const TArray<FName> QuestIDs)
{
	FString ContextString;

	for (FName QuestID : QuestIDs) //for every ID we try and find it in the data table
	{
		if (FQuest* QuestPtr = QuestList->FindRow<FQuest>(QuestID, ContextString))
		{
			//If we find the quest, add to array, execute first objective event and then update the UI
			ActiveQuests.AddUnique(*QuestPtr);

			//Manually trigger first event since it's our first
			ExecuteQuestEvent(QuestPtr->Objectives[0].EventName);

			if (PlayerCon)
			{
				PlayerCon->UpdateQuestNameUI.Broadcast(QuestPtr->Name);
			}
		}
	}
}

void AQuestline::InitialiseQuests()
{
	if (QuestList)
	{
		//Get the player controller/HUD and save so we don't have to keep doing it
		if (UWorld* World = GetWorld())
		{
			PlayerCon = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));

			if (PlayerCon)
			{
				GameHUD = Cast<AGameHUD>(PlayerCon->GetHUD());
			}
		}

		TArray<FName> RowNames = QuestList->GetRowNames();
		FString ContextString;

		for (FName ID : RowNames)
		{

			if (FQuest* QuestPtr = QuestList->FindRow<FQuest>(ID, ContextString))
			{
				if (QuestPtr->bActivateOnBeginPlay)
				{
					//Create array just to read in ID value
					TArray<FName> IDs;
					IDs.Add(ID);
					ActivateQuests(IDs);

					if (QuestActorRefs.Find(QuestPtr->Objectives[0].ObjectiveID)) //If I can find this actor
					{
						if (GameHUD && PlayerCon)
						{
							GameHUD->UpdateObjectiveValues(*QuestActorRefs.Find(QuestPtr->Objectives[0].ObjectiveID)); //then dereference it and update
							PlayerCon->UpdateMessageUI.Broadcast(QuestPtr->Objectives[0].Description, 5.f);
						}
					}
				}
			}
		}
	}

	return;
}

bool AQuestline::GetHighestPriorityQuest(const TArray<FName>& QuestIDs, FQuest& OutQuest)
{
	int32 BestScore = 0;
	bool bFoundQuest = false;

	for (const FQuest& Quest: ActiveQuests)
	{
		for (const FName ID : QuestIDs) //Nested loop for IDs we need to see if each quest matches up the IDs passed in
		{
			if (Quest.ID == ID)
			{
				if (Quest.Priority >= BestScore) 
				{
					//Make this the new out quest
					OutQuest = Quest;

					//Update the best score
					BestScore = Quest.Priority;

					//We found a quest; all is well, we'll have something to return
					bFoundQuest = true;
				}
			}
		}
	}

	return bFoundQuest;
}

bool AQuestline::IsQuestArchived(FName QuestID) const
{
	for (const FQuest& Quest : ArchivedQuests)
	{
		if (Quest.ID == QuestID)
		{
			return true;
		}
	}

	return false;
}

bool AQuestline::IsCurrentSelectedQuest(const int32 QuestIndex) const
{
	if (PlayerCon)
	{
		if (QuestIndex == PlayerCon->GetQuestActiveIndex())
		{
			return true;
		}
	}
		
	return false;
}

bool AQuestline::AtStartOfFinalObjective(const int32 ObjectivesLength, const int32 ObjectiveIndex) const
{
	return (ObjectivesLength == ObjectiveIndex);
}

void AQuestline::UpdateObjectiveTracker(const FString QuestActorID)
{
	if (GameHUD)
	{
		if (QuestActorRefs.Find(QuestActorID))
		{
			GameHUD->UpdateObjectiveValues(*QuestActorRefs.Find(QuestActorID));
		}
	}
}

void AQuestline::DeactivateQuest(const FQuest& QuestToArchive)
{
	ArchivedQuests.Add(QuestToArchive);
	ActiveQuests.Remove(QuestToArchive);

	if (PlayerCon)
	{
		//Set to unavailable quest
		PlayerCon->SetQuestActiveIndex(-1);

		if (GameHUD)
		{
			GameHUD->ClearObjectiveValues();
		}
	}
}

//since only called by questoutcome, this outquest will always be the correct one
void AQuestline::MoveOnToNextObjective(FQuest& OutQuest) 
{

	FObjective Objective;
	FObjective NextObjective;
	bool bShouldArchive = false;

	for (int32 i = 0; i < OutQuest.Objectives.Num() - 1 ; i++) //going up to but not including last index
	{
		Objective = OutQuest.Objectives[i];
		NextObjective = OutQuest.Objectives[i + 1];

		if (Objective.Stage == OutQuest.Stage)
		{
			if (AtStartOfFinalObjective((OutQuest.Objectives.Num()-1), i + 1))
			{
				bShouldArchive = true;
			}
			else
			{
				OutQuest.Stage = NextObjective.Stage; //grab the next stage and set to as current
			}

			//in any case we're going to be calling the quest event associated
			ExecuteQuestEvent(NextObjective.EventName);

			//Get the index of the quest we put in and update any UIs
			if (IsCurrentSelectedQuest(ActiveQuests.Find(OutQuest)))
			{
				if (PlayerCon)
				{
					PlayerCon->UpdateMessageUI.Broadcast(NextObjective.Description, 5.f);
					UpdateObjectiveTracker(NextObjective.ObjectiveID);
				}
			}

			if (bShouldArchive)
			{
				DeactivateQuest(OutQuest);
				return;
			}
			else
			{
				int32 Index = ActiveQuests.Find(OutQuest);
				if (ActiveQuests.IsValidIndex(Index))
				{
					ActiveQuests[Index] = OutQuest;
				}
				return;
			}
		}
	}

	return;
}

void AQuestline::QuestOutcomeAction(const FName ID, const EDialogueOutcome OutcomeAction)
{
	FQuest QuestToUpdate;
	bool bFound = false;

	for (const FQuest& Quest : ActiveQuests)
	{
		if (ID == Quest.ID)
		{
			if (OutcomeAction == EDialogueOutcome::EUpdateObjective)
			{
				QuestToUpdate = Quest;
				bFound = true;
			}
		}
	}

	//Don't do it inside loop because we might have to remove a quest from the active quests array
	if (bFound)
	{
		MoveOnToNextObjective(QuestToUpdate);
	}
}

void AQuestline::GetActiveQuests(TArray<FQuest>& ActiveQuestsGet) const
{
	ActiveQuestsGet = ActiveQuests;
}

void AQuestline::GetArchivedQuests(TArray<FQuest>& ArchivedQuestsGet) const
{
	ArchivedQuestsGet = ArchivedQuests;
}
	