// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Public/GamePlayerController.h"
#include "Public/InventoryComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APlayerCharacter::GetInventoryItem(const TSubclassOf<AActor> ActorClass, FInventoryItem& Item)
{
	for (const FInventoryItem InventoryItem : Inventory)
	{
		if (InventoryItem.ActorClass == ActorClass)
		{
			Item = InventoryItem;
			return true;
		}
	}

	return false;
}

void APlayerCharacter::GetInventory(TArray<FInventoryItem>& InventoryToGet) const
{
	InventoryToGet = Inventory;
}

AActor* APlayerCharacter::PlaceInventoryItem(FInventoryItem& Item, const FTransform Transform, int32& Count)
{
	Count = RemoveInventoryItem(Item);

	if (UWorld* World = GetWorld())
	{
		if (Count >= 0) //Reflects count after we've just removed the item
		{
			FActorSpawnParameters SpawnParameters;
			return World->SpawnActor(Item.ActorClass, &Transform);
		}
		else
		{
			if (AGamePlayerController* PlayerCon = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
			{
				PlayerCon->UpdateMessageUI.Broadcast(FText::FromString("No item in inventory to place!"), 5.f);
			}
		}
	}

	//Item count isn't greater than 0, so we can't place it
	return nullptr;
}

AActor* APlayerCharacter::EquipItem(FInventoryItem& InventoryItem)
{
	if (UWorld* World = GetWorld())
	{
		//Spawn actor in world
		FVector LocationToPlace = FVector(0);
		AActor* ActorRef = World->SpawnActor(InventoryItem.ActorClass, &LocationToPlace);

		if (ActorRef)
		{
			EquippedItems.Add(ActorRef);

			//Find the index of the item in the inventory
			int32 Index = Inventory.Find(InventoryItem);

			//Read this index into the equipped items
			SetEquippedItem(Index, true);
			InventoryItem.bCurrentlyEquipped = true;

			return ActorRef;
		}
	}

	return nullptr;
}

//Remove the equipped item (seperate function for this as we have an equipped items array)
void APlayerCharacter::RemoveEquippedItem(FInventoryItem& ItemToRemoveEquip)
{
	AActor* ItemToRemove = nullptr;

	//Get the item in the equipped array and remove from array
	for (AActor* EquippedItem : EquippedItems)
	{
		if (EquippedItem->GetClass() == ItemToRemoveEquip.ActorClass)
		{
			ItemToRemove = EquippedItem;
		}
	}


	if (ItemToRemove)
	{
		//Remove item from equipped items
		EquippedItems.Remove(ItemToRemove);

		int32 Index = Inventory.Find(ItemToRemoveEquip);

		//Make equipped item false
		SetEquippedItem(Index, false);
		ItemToRemoveEquip.bCurrentlyEquipped = false;

		//Destroy item for good
		ItemToRemove->Destroy();
	}

	return;
}

void APlayerCharacter::SetEquippedItem(int32 Index, bool bEquipped)
{
	if (Inventory.IsValidIndex(Index))
	{
		Inventory[Index].bCurrentlyEquipped = bEquipped;
	}

	return;
}

int32 APlayerCharacter::RemoveInventoryItem(FInventoryItem& Item)
{
	int32 Count = -1;

	//If it's currently equipped then remove
	if (Item.bCurrentlyEquipped)
	{
		RemoveEquippedItem(Item);
	}

	//If the item count is greater than 1 then remove 1 from the counter
	if (Item.ItemCount > 1)
	{
		int32 Index = Inventory.Find(Item);
		Item.ItemCount = Item.ItemCount - 1; //to reflect change in the UI
		Count = AugmentInventoryItemCount(Index, -1);
	}
	else
	{
		//if it's the last then remove completely
		Inventory.Remove(Item);
		Count = 0;
	}

	return Count;
}

int32 APlayerCharacter::AugmentInventoryItemCount(int32 Index, int32 Increment)
{
	if (Inventory.IsValidIndex(Index))
	{
		Inventory[Index].ItemCount = Inventory[Index].ItemCount + Increment;

		//Return the augmented count
		return Inventory[Index].ItemCount;
	}

	return -1;
}

void APlayerCharacter::AddInventoryItem(FInventoryItem& ItemToAdd)
{
	int32 Index = Inventory.Find(ItemToAdd);

	if (Inventory.IsValidIndex(Index))
	{
		//read in index and increment count of already present items
		ItemToAdd.ItemCount += 1;
		AugmentInventoryItemCount(Index, +1);
	}
	else
	{
		//if you can't find the item then add a new one
		Inventory.Add(ItemToAdd);
	}
}

bool APlayerCharacter::FindInventoryItemByClass(const TSubclassOf<AActor> ActorClass, FInventoryItem& ItemToFill)
{
	for (FInventoryItem Item : Inventory)
	{
		if (Item.ActorClass == ActorClass)
		{
			ItemToFill = Item;
			return true;
		}
	}

	return false;
}

void APlayerCharacter::AddInventoryByClass(TSubclassOf<AActor> ActorClass, int32 Count)
{
	if (UWorld* World = GetWorld())
	{
		FVector LocationToPlace = FVector(0);

		if (AActor* ActorRef = World->SpawnActor(ActorClass, &LocationToPlace))
		{
			//If the actor has an inventory component then add to the player inventory
			if (UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(ActorRef->GetComponentByClass(UInventoryComponent::StaticClass())))
			{
				InventoryComp->SetItemClass(ActorClass);
				InventoryComp->SetItemCount(Count);
				InventoryComp->AddToPlayerInventory();
				ActorRef->Destroy();
			}
		}
	}
}