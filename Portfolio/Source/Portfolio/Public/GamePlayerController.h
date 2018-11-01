// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Public/Interactable.h"
#include "Public/DialogueComponent.h"
#include "Public/BlueprintableStructs.h"
#include "GamePlayerController.generated.h"

class UUserWidget;
class UDataTable;
class AGameHUD;

//Delegates for UI updates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateJournalUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateMessageUI, const FText, MessageText, const float, MessageDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnUpdateDialogueUI, const TArray<FDialogue>&, Dialogue, const FName, QuestID, const UDataTable*, Outcomes, const int32, Stage, const UDialogueComponent*, DialogueComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateActiveQuest, const int32, QuestIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuestName, const FText, QuestName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateInventoryMessageUI, const FText, MessageText, const float, MessageDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateCasualDialogueUI, const FString, ActorName, const FString, Dialogue, const float, DialogueDuration);


/**
 * 
 */
UCLASS()
class PORTFOLIO_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	//Constructor
	AGamePlayerController();

	/* UI */

	//We call this in our Game Mode so that the UI is not created before the initial quests are gathered
	void CreateInitialUI();

	/* Interactables */

	//We've encountered an interactable and want it to be considered (will call this in player character)
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractableEncountered(AActor* Interactable);

	//We've left an interactable and now we want to remove it from the array (will call this in player character)
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractableEnded(AActor* Interactable);

	/* Dialogue */

	UFUNCTION(BlueprintImplementableEvent)
	void StartDialogue(const TArray<FDialogue>& Dialogue, const UDialogueComponent* DialogueComponent, const FName ID, const UDataTable* Outcomes, const int32 Stage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StopDialogue(const UDialogueComponent* DialogueComponent);

	/* Delegates */

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Message UI")
	FOnUpdateMessageUI UpdateMessageUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Dialogue UI")
	FOnUpdateDialogueUI UpdateDialogueUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Quest UI")
	FOnUpdateActiveQuest UpdateActiveQuestUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Quest UI")
	FOnUpdateQuestName UpdateQuestNameUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Inventory UI")
	FOnUpdateInventoryMessageUI UpdateInventoryMessageUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Dialogue UI")
	FOnUpdateCasualDialogueUI UpdateCasualDialogueUI;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Journal UI")
	FOnUpdateJournalUI UpdateJournalUI;

	/* Quest */

	UFUNCTION(BlueprintCallable, Category = "Quest Getters")
	int32 GetQuestActiveIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Quest Setters")
	void SetQuestActiveIndex(const int32 NewActiveQuestIndex);
	
protected:

	virtual void BeginPlay() override;

	/* UI variables and functions */

	//Journal variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal UI")
	TSubclassOf<UUserWidget> JournalWidgetClass;
	UPROPERTY()
	UUserWidget* JournalWidget;
	UFUNCTION(BlueprintCallable, Category = "Journal UI")
	void CreateJournalUI();
	UFUNCTION(BlueprintCallable, Category = "Journal UI")
	void RemoveJournalUI();

	//Player variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;
	UPROPERTY()
	UUserWidget* PlayerWidget;


	/* Interactable variables and functions */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractableUpdateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float AcceptableDotProduct;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable;

	//Make a call to the HUD and feed it the values of the interactable we just encountered
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionUI();

	
	/* Quest variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI")
	int32 QuestActiveIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest UI")
	int32 ActiveTabIndex;

private:

	/* Interactable variables and functions */

	UPROPERTY()
	TArray<AActor*> Interactables;
	FTimerHandle InteractionTimerHandle;
	void CheckForBestInteractable();
	void GetDotProducts(TArray<float>& AcceptableDots);

	/* Other variables and functions */

	UPROPERTY()
	AGameHUD* GameHUD;
	UPROPERTY()
	APawn* ControlledPawn;	
};
