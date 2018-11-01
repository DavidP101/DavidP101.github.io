// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Public/PlayerCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddToPlayerInventory()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0)))
		{
			PlayerChar->AddInventoryItem(InventoryItem);
		}
	}
}

void UInventoryComponent::SetItemCount(const int32 Count)
{
	InventoryItem.ItemCount = Count;
}

void UInventoryComponent::SetItemClass(const TSubclassOf<AActor> ActorClass)
{
	InventoryItem.ActorClass = ActorClass;
}
