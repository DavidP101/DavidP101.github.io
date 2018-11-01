// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Font.h"

void AGameHUD::UpdateInteractionValues(FString Text, FVector Offset, USceneComponent* SceneComp)
{
	if (SceneComp)
	{
		CurrentInteractable = SceneComp;
	}

	InteractionText = Text;
	InteractionOffset = Offset;
}

void AGameHUD::UpdateObjectiveValues(AActor* ObjectiveActor)
{
	if (ObjectiveActor)
	{
		CurrentObjective = ObjectiveActor;
	}
}

void AGameHUD::ClearObjectiveValues()
{
	CurrentObjective = nullptr;
}

void AGameHUD::ClearInteractionData()
{
	CurrentInteractable = nullptr;
	InteractionText = FString();
	InteractionOffset = FVector(0);
}

void AGameHUD::DrawObjectiveMarker(int32 SizeX, int32 SizeY)
{
	if (CurrentObjective)
	{
		//Get the player pawn
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			//Apply an offset and save as integer
			int32 RoundedLength = (((PlayerPawn->GetActorLocation() - CurrentObjective->GetActorLocation()).Size())/100);

			//3D to 2D Screen Coords
			FVector ProjectedLocation = Project(CurrentObjective->GetActorLocation());

			if (TextFont)
			{
				if (ProjectedLocation.Z > 0)
				{
					//First we need to draw the objective text, this will be how far we are from the objective
					FString ObjectiveText = FString::FromInt(RoundedLength).Append("m");
					float ObjectiveTextX = CentreText(ProjectedLocation.X, ObjectiveText);
					DrawText(ObjectiveText, TextColour, ObjectiveTextX, ProjectedLocation.Y, TextFont, 1.f);

					if (ObjectiveTexture) //making sure we have a texture
					{
						//Next we'll need to draw the objective marker (texture)
						float ObjectiveTextureX = CentreText(ProjectedLocation.X, FString::FromInt(RoundedLength));
						DrawTextureSimple(ObjectiveTexture, ObjectiveTextureX, (ProjectedLocation.Y - 25), 0.375f);
					}
				}
			}

		}
	}
}

void AGameHUD::DrawInteractionText(int32 SizeX, int32 SizeY)
{
	if (CurrentInteractable)
	{
		FVector ProjectedLocationWithOffset = Project(CurrentInteractable->GetComponentLocation() + InteractionOffset);

		if (TextFont)
		{
			if (ProjectedLocationWithOffset.Z > 0)
			{
				float InteractionTextX = CentreText(ProjectedLocationWithOffset.X, InteractionText);
				DrawText(InteractionText, TextColour, InteractionTextX, (ProjectedLocationWithOffset.Y-50), TextFont);
			}
		}
	}
}

float AGameHUD::CentreText(float ScreenX, FString TextToCentre)
{
	float OutWidth = 0.f;
	float OutHeight = 0.f;
	GetTextSize(TextToCentre, OutWidth, OutHeight, TextFont);
	return (ScreenX - (OutWidth / 2));
}
