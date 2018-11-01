// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageComponent.h"
#include "Engine/World.h"
#include "GamePlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMessageComponent::UMessageComponent()
{
}


// Called when the game starts
void UMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		PlayerCon = Cast<AGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
	}
}

void UMessageComponent::CallMessageUI(const FText Message, const float MessageDuration)
{
	if (PlayerCon)
	{
		PlayerCon->UpdateMessageUI.Broadcast(Message, MessageDuration);
	}
}

void UMessageComponent::CallInventoryMessageUI(const FText Message, const float MessageDuration)
{
	if (PlayerCon)
	{
		PlayerCon->UpdateInventoryMessageUI.Broadcast(Message, MessageDuration);
	}
}

FText UMessageComponent::GetMessageText(const int32 Index) const
{
	if (MessageText.IsValidIndex(Index))
	{
		return MessageText[Index];
	}

	return FText::FromString("There's supposed to be custom text here, whoops!");
}



