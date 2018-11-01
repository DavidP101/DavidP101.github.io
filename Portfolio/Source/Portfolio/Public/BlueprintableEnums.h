#pragma once

#include "CoreMinimal.h"
#include "BlueprintableEnums.generated.h"

UENUM(BlueprintType)
enum class EDialogueOutcome : uint8
{
	EUpdateObjective UMETA(DisplayName = "Update Objective"),
	EExit UMETA(DisplayName = "Exit")
};