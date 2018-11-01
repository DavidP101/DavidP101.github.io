// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/BlueprintableStructs.h"
#include "Public/BlueprintableEnums.h"
#include "Questline.generated.h"

UCLASS()
class PORTFOLIO_API AQuestline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestline();

	//Get the Actor associated with the current objective
	UFUNCTION(BlueprintCallable, Category = "Objectives")
	AActor* GetObjectiveActor(const int32 QuestIndex);

	//Will be called by the Dialogue Component to fetch highest priority dialogue
	UFUNCTION(BlueprintCallable, Category = "Questline")
	bool GetHighestPriorityQuest(const TArray<FName>& QuestIDs, FQuest& OutQuest);

	//Will be called by the Game Mode
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void InitialiseQuests();

	//Check to see if a quest is archived (called by Quest Interaction Volume)
	UFUNCTION(BlueprintCallable, Category = "Questline Checks")
	bool IsQuestArchived(const FName QuestID) const;

	//Used in the UI for highlighting
	UFUNCTION(BlueprintCallable, Category = "Questline Checks")
	bool IsCurrentSelectedQuest(const int32 QuestIndex) const;

	//Used in the UI for updating objective texture location
	UFUNCTION(BlueprintCallable, Category = "Questline Objectives")
	void UpdateObjectiveTracker(const FString QuestActorID);

	//Called everywhere for updating quests
	UFUNCTION(BlueprintCallable, Category = "Questline Objectives")
	void QuestOutcomeAction(const FName ID, const EDialogueOutcome OutcomeAction);

	//Get all the quests that currently active to the player
	UFUNCTION(BlueprintCallable, Category = "Questline Getters")
	void GetActiveQuests(TArray<FQuest>& ActiveQuestsGet) const;

	//Get all the quests that the player has completed
	UFUNCTION(BlueprintCallable, Category = "Questline Getters")
	void GetArchivedQuests(TArray<FQuest>& ArchivedQuestsGet) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Contains datatable detailing all the quests
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Questline")
	class UDataTable* QuestList;

	//Reference to various quest actors in the level (manually set through editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Actor References")
	TMap<FString, AActor*> QuestActorRefs;

	//Will call the events specified in the datatable
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Questline Getters")
	void ExecuteQuestEvent(const FString& QuestEvent);

	//Activates a quest i.e. moves from data table to active quests array
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void ActivateQuests(const TArray<FName> QuestIDs);

private:

	TArray<FQuest> ActiveQuests;
	TArray<FQuest> ArchivedQuests;

	//Called by QuestOutcomeAction only to move on to the next objective
	void MoveOnToNextObjective(FQuest& OutQuest);

	//Called by MoveOnToNextObjective only to see if we're at the start of the final objective
	bool AtStartOfFinalObjective(const int32 ObjectivesLength, const int32 ObjectiveIndex) const;

	//Called by MoveOnToNextObjective only to deactivate a quest (move from active to archived)
	void DeactivateQuest(const FQuest& QuestToArchive);

	//so we don't have to keep fetching it
	UPROPERTY()
	class AGamePlayerController* PlayerCon;
	UPROPERTY()
	class AGameHUD* GameHUD;
};
