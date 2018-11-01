// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Public/GameHUD.h"
#include "Engine/DataTable.h"
#include "Public/BlueprintableStructs.h"
#include "Public/Blueprint/UserWidget.h"
#include "Questline.h"
#include "UObject/ScriptInterface.h"
#include "NewQuestGM.h"
#include "TimerManager.h"

AGamePlayerController::AGamePlayerController()
{
	InteractableUpdateTime = 0.03f;
	AcceptableDotProduct = 0.6f;
}

int32 AGamePlayerController::GetQuestActiveIndex() const
{
	return QuestActiveIndex;
}

void AGamePlayerController::SetQuestActiveIndex(const int32 NewActiveQuestIndex)
{
	QuestActiveIndex = NewActiveQuestIndex;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Save the Game HUD so we can use it later
	if (UWorld* World = GetWorld())
	{
		if (AHUD* BaseHUD = UGameplayStatics::GetPlayerController(World, 0)->GetHUD())
		{
			GameHUD = Cast<AGameHUD>(BaseHUD);
		}
	}

	ControlledPawn = GetPawn();
}

void AGamePlayerController::CreateInitialUI()
{
	if (PlayerWidgetClass)
	{
		if (!PlayerWidget) //if the object doesn't already exist
		{
			PlayerWidget = CreateWidget<UUserWidget>(this, PlayerWidgetClass);
			PlayerWidget->AddToViewport(0);
		}
	}
}

void AGamePlayerController::CheckForBestInteractable()
{
	TArray<float> AcceptableDots;

	//Add acceptable interactables to the dot products array
	GetDotProducts(AcceptableDots);

	//assuming we've found one
	if (AcceptableDots.Num() > 0)
	{
		int32 BestInteractableIndex = 0;
		float TempVal = 0.f;

		//will find best interactable in the array
		for (int32 i = 0; i < AcceptableDots.Num(); i++)
		{
			if (AcceptableDots[i] >= TempVal)
			{
				//Make this the new value
				TempVal = AcceptableDots[i];

				//save the index
				BestInteractableIndex = i;
			}
		}

		//If the interactable we get back is not one we've encountered directly before
		if (Interactables[BestInteractableIndex] != CurrentInteractable)
		{
			//Update interaction values
			if (GameHUD)
			{
				//Set best interactable as the new interactable
				CurrentInteractable = Interactables[BestInteractableIndex];
				UpdateInteractionUI();
			}
		}

	}
}

//If we're in the presence of multiple interactables, do gatekeeping to see which ones are valid
void AGamePlayerController::GetDotProducts(TArray<float>& AcceptableDots)
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0))
		{
			for (AActor* Interact : Interactables)
			{
				float Dot = FVector::DotProduct(CameraManager->GetActorForwardVector(), (Interact->GetActorLocation() - CameraManager->GetCameraLocation()).GetSafeNormal());

				if (Dot > AcceptableDotProduct)
				{
					AcceptableDots.Add(Dot);
				}
			}
		}
	}
}
	
void AGamePlayerController::UpdateInteractionUI()
{
	if (CurrentInteractable)
	{
		if (CurrentInteractable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			FInteractionData InteractionData;
			IInteractable::Execute_GetInteractionData(CurrentInteractable, InteractionData);

			if (GameHUD)
			{
				GameHUD->UpdateInteractionValues(InteractionData.InteractionText.ToString(), InteractionData.WidgetOffset, InteractionData.WidgetComp);
			}
		}
	}
}

void AGamePlayerController::InteractableEncountered(AActor* Interactable)
{
	if (Interactable)
	{
		if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			Interactables.AddUnique(Interactable);
			GetWorldTimerManager().SetTimer(InteractionTimerHandle, this, &AGamePlayerController::CheckForBestInteractable, InteractableUpdateTime, true);
		}
	}

	return;
}

void AGamePlayerController::InteractableEnded(AActor* Interactable)
{
	if (Interactable)
	{
		if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			Interactables.Remove(Interactable);

			if (Interactables.Num() <= 0)
			{
				//invalidate the handle, this stops the timer
				InteractionTimerHandle.Invalidate();

				if (GameHUD)
				{
					GameHUD->ClearInteractionData();
				}

				//We can't be interacting with anything anymore if there's nothing left
				CurrentInteractable = nullptr;
			}

		}
	}
}

void AGamePlayerController::CreateJournalUI()
{
	if (JournalWidgetClass)
	{
		if (!JournalWidget)
		{
			if (UWorld* World = GetWorld())
			{
				UGameplayStatics::SetGamePaused(World, true);
				JournalWidget = CreateWidget<UUserWidget>(this, JournalWidgetClass);
				JournalWidget->AddToViewport(0);
				SetInputMode(FInputModeGameAndUI());
				bShowMouseCursor = true;
				UpdateJournalUI.Broadcast();
			}
		}
	}
}

void AGamePlayerController::RemoveJournalUI()
{
	if (JournalWidget)
	{
		if (UWorld* World = GetWorld())
		{
			UGameplayStatics::SetGamePaused(World, false);
			JournalWidget->RemoveFromViewport();
			JournalWidget = nullptr;
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
}