// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/BlueprintableStructs.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	//Inventory item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FInventoryItem InventoryItem;

public:

	UFUNCTION(BlueprintCallable, Category = "Player Inventory")
	void AddToPlayerInventory();

	UFUNCTION(BlueprintCallable, Category = "Player Inventory")
	void SetItemCount(const int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Player Inventory")
	void SetItemClass(const TSubclassOf<AActor> ActorClass);
};
