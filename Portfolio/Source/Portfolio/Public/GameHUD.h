// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionValues(FString Text, FVector Offset, USceneComponent* SceneComp);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void UpdateObjectiveValues(AActor* ObjectiveActor);

	//Clear any interaction that may be on the HUD
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ClearInteractionData();

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void ClearObjectiveValues();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	AActor* CurrentObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	USceneComponent* CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void DrawObjectiveMarker(int32 SizeX, int32 SizeY);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DrawInteractionText(int32 SizeX, int32 SizeY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Font")
	UFont* TextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* ObjectiveTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Font")
	FLinearColor TextColour;

private:
	float CentreText(float ScreenX, FString TextToCentre);

	//These variables will be set in Update methods
	FString InteractionText;
	FVector InteractionOffset;
};
