// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/DialogueComponent.h"
#include "Engine/DataTable.h"
#include "Public/NewQuestGM.h"
#include "Kismet/GameplayStatics.h"
#include "Public/GamePlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Public/Questline.h"
#include "Engine/World.h"
#include "Public/BlueprintableStructs.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::PackageDialogue(TArray<FDialogue>& DialogueArray, const UDataTable* DialogueTable, const int32 Stage)
{
	if (DialogueTable)
	{
		TArray<FName> RowNames = DialogueTable->GetRowNames();
		FString ContextString;
		for (FName TableRow : RowNames)
		{
			if (DialogueTable->FindRow<FDialogue>(TableRow, ContextString)->Stage == Stage)
			{
				FDialogue* SingleDialogueRow = (DialogueTable->FindRow<FDialogue>(TableRow, ContextString));

				if (SingleDialogueRow) //if we do find the row then add it to the array
				{
					DialogueArray.Add(*SingleDialogueRow);
				}
			}
		}
	}

	return;
}

bool UDialogueComponent::HandleDialogue()
{
	if (HandleQuestDialogue())
	{
		return false;
	}
	else if (HandleGeneralDialogue())
	{
		return false;
	}
	else
	{
		StartCasualDialogue();
		return true;
	}

}

bool UDialogueComponent::HandleQuestDialogue()
{
	TArray<FName> Keys;
	QuestDialogue.GetKeys(Keys);
	FQuest Quest;

	if (QuestDialogue.Num() > 0)
	{
		if (ANewQuestGM* GameMode = Cast<ANewQuestGM>(GetWorld()->GetAuthGameMode()))
		{
			if (AQuestline* Questline = GameMode->GetQuestline())
			{
				//Check the questline for the highest priority quest that's available (no guarantees there will be any)
				if (Questline->GetHighestPriorityQuest(Keys, Quest))
				{
					if (FQuestEntry* QuestEntry = QuestDialogue.Find(Quest.ID))
					{
						//Holds the dialogue we're going to use for the chat
						TArray<FDialogue> QuestDialogueArray;

						PackageDialogue(QuestDialogueArray, QuestEntry->Dialogue, Quest.Stage);

						if (QuestDialogueArray.Num() > 0)
						{
							StartDialogue(QuestDialogueArray, Quest.ID, QuestEntry->Outcomes, Quest.Stage);
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
	}

	return false;
}

bool UDialogueComponent::HandleGeneralDialogue()
{
	if (GeneralDialogue.Dialogue && GeneralDialogue.Outcomes)
	{
		TArray<FDialogue> GeneralDialogueArray;

		PackageDialogue(GeneralDialogueArray, GeneralDialogue.Dialogue, 0);

		if (GeneralDialogueArray.Num() > 0)
		{
			StartDialogue(GeneralDialogueArray, "None", GeneralDialogue.Outcomes, 0);
			return true;
		}
	}

	return false;
}

void UDialogueComponent::StartDialogue(const TArray<FDialogue>& Dialogue, const FName ID, const UDataTable* Outcomes, const int32 Stage) const
{
	if (UWorld* World = GetWorld())
	{
		if (AGamePlayerController* PlayerController = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			PlayerController->StartDialogue(Dialogue, this, ID, Outcomes, Stage);
		}
	}
}

void UDialogueComponent::StopDialogue() const
{
	OnStopDialogue.Broadcast();
}

void UDialogueComponent::StartCasualDialogue() const
{
	if (UWorld* World = GetWorld())
	{
		if (AGamePlayerController* PlayerController = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			if (DialogueOptions.Num() > 0)
			{
				int32 RandIndex = FMath::RandRange(0, DialogueOptions.Num() - 1);
				PlayerController->UpdateCasualDialogueUI.Broadcast(DialogueName.ToString(), DialogueOptions[RandIndex], 10.f);
			}
		}
	}
}

