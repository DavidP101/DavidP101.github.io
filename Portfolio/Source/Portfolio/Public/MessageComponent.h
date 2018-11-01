// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MessageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMessageComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	TArray<FText> MessageText;

public:

	UFUNCTION(BlueprintCallable, Category = "Message Calling")
	void CallMessageUI(const FText Message, const float MessageDuration = 5.f);

	UFUNCTION(BlueprintCallable, Category = "Message Calling")
	void CallInventoryMessageUI(const FText Message, const float MessageDuration = 5.f);

	UFUNCTION(BlueprintCallable, Category = "Message Calling")
	FText GetMessageText(const int32 Index) const;

private:

	//defines the player controller to use so that we can call our messages
	UPROPERTY()
	class AGamePlayerController* PlayerCon;
};
