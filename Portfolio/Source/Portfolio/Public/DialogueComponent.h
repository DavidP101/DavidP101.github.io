// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/BlueprintableStructs.h"
#include "DialogueComponent.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopDialogue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

	//We call this in the AI character so we can know when dialogue ends
	UPROPERTY(BlueprintAssignable, Category = "Stop Dialogue")
	FOnStopDialogue OnStopDialogue;

	//Handle the dialogue, returning a bool for whether or not the dialogue is casual or advanced
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool HandleDialogue();

	//Stops dialogue, called by player controller will broadcast stop dialogue delegate so that any interested parties may act
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StopDialogue() const;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	//Package any found dialogue into a format suitable for the UI
	void PackageDialogue(TArray<FDialogue>& DialogueArray, const UDataTable* DialogueTable, const int32 Stage);

	/* Dialogue related variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Dialogue")
	TMap<FName, FQuestEntry> QuestDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Dialogue")
	FQuestEntry GeneralDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casual Dialogue")
	TArray<FString> DialogueOptions;

	//Name of the character for casual dialogue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DialogueName;

private:

	bool HandleQuestDialogue();
	bool HandleGeneralDialogue();
	void StartCasualDialogue() const;
	void StartDialogue(const TArray<FDialogue>& Dialogue, const FName ID, const UDataTable* Outcomes, const int32 Stage) const;
};
