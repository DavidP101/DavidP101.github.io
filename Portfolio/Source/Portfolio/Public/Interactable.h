// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Public/BlueprintableStructs.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTFOLIO_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	bool Interact();

	//Get the data associated with the interactable
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void GetInteractionData(FInteractionData& InteractionData);

	//exit the interaction
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void ExitInteract();
	
};
