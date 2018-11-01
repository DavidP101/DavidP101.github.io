// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/BlueprintableStructs.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PORTFOLIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Find inventory item and fill reference
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetInventoryItem(const TSubclassOf<AActor> ActorClass, UPARAM(ref) FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 RemoveInventoryItem(UPARAM(ref) FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Equipped")
	void RemoveEquippedItem(UPARAM(ref) FInventoryItem& ItemToRemoveEquip);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetInventory(TArray<FInventoryItem>& InventoryToGet) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AActor* PlaceInventoryItem(UPARAM(ref) FInventoryItem& Item, const FTransform Transform, int32& Count);

	UFUNCTION(BlueprintCallable, Category = "Equipped")
	AActor* EquipItem(UPARAM(ref) FInventoryItem& InventoryItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindInventoryItemByClass(const TSubclassOf<AActor> ActorClass, FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInventoryByClass(TSubclassOf<AActor> ActorClass, int32 Count);

	void AddInventoryItem(UPARAM(ref) FInventoryItem& ItemToAdd);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> Inventory;

private:
	TArray<AActor*> EquippedItems;

	//If it's not the last item then we just want to decrease a counter
	int32 AugmentInventoryItemCount(int32 Index, int32 Increment);

	//Set the equipped status to true or false, will be called by Equip/Unequip respectively
	void SetEquippedItem(int32 Index, bool bEquipped);
};
