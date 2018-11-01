#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Public/BlueprintableEnums.h"
#include "Gameframework/Actor.h"
#include "Engine/Texture2D.h"
#include "BlueprintableStructs.generated.h"

//Defines the struct for our psuedo 'branching' dialogue
USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Struct")
	int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Struct")
	TArray<FString> NPCResponses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Struct")
	TArray<FString> PlayerResponses;

	FDialogue() : Stage(0), NPCResponses(), PlayerResponses() {}
};

//Defines the struct for any interactable data
USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Struct")
	FVector WidgetOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Struct")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Struct")
	USceneComponent* WidgetComp;

	FInteractionData() : WidgetOffset(0), InteractionText(), WidgetComp() {}
};

//Defines the struct for a quest entry
USTRUCT(BlueprintType)
struct FQuestEntry : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	UDataTable* Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	UDataTable* Outcomes;

	FQuestEntry() : Dialogue(), Outcomes() {}
};

//Defines the struct for an objective
USTRUCT(BlueprintType)
struct FObjective : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Struct")
	int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Struct")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Struct")
	FString EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Struct")
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Struct")
	bool bDesignerOnlyView;

	FObjective() : Stage(0), Description(), EventName("None"), ObjectiveID(), bDesignerOnlyView(false) {}
};

//Defines the struct for a quest
USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	TArray<FObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	bool bActivateOnBeginPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Struct")
	int32 Priority;

	FQuest() : ID(), Name(), Objectives(), bActivateOnBeginPlay(false), Stage(0), Priority(50) {}
};

inline bool operator==(const FQuest& V1, const FQuest& V2)
{
	return V1.ID == V2.ID;
}

//Defines the struct for a quest outcome
USTRUCT(BlueprintType)
struct FQuestOutcome : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Outcome Struct")
	int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Outcome Struct")
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Outcome Struct")
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Outcome Struct")
	TArray<EDialogueOutcome> DialogueOutcomes;

	FQuestOutcome() : Stage(0), Row(0), Index(0), DialogueOutcomes() {}
};

//Defines the struct for an inventory item
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	bool bQuestItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	bool bEquippable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Struct")
	bool bCurrentlyEquipped;

	FInventoryItem() : Description(), Icon(), ActorClass(AActor::StaticClass()), bQuestItem(false), ItemCount(1), bEquippable(false), bCurrentlyEquipped(false) {}
};

//When we search for an item we'll do it by the class
inline bool operator==(const FInventoryItem& V1, const FInventoryItem& V2)
{
	return V1.ActorClass == V2.ActorClass;
}