// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayController.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interactable.h"
#include "HeroBrushCharacter.h"

void AGameplayController::AddItemToInventoryByID(FName ID)
{
	
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	UDataTable* ItemTable = GameMode->GetItemDB();
	
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	UE_LOG(LogTemp, Warning, TEXT("ID:%s"), *ID.ToString());
	if (ItemToAdd) {
		Inventory.Add(*ItemToAdd);
	}
}

void AGameplayController::Interact()
{
	
	if (CurrentInteractable) {
		CurrentInteractable->Interact(this);
	}
}

void AGameplayController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Use", IE_Pressed, this, &AGameplayController::Interact);
}

void AGameplayController::CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, AGameplayController* controller)
{
	for (auto Craft : ItemB.CraftCombinations) {
		
		if (Craft.ComponentID == ItemA.ItemID) {
			if (Craft.bDestroyItemA) {
				Inventory.RemoveSingle(ItemA);
			}
			if (Craft.bDestroyItemB) {
				Inventory.RemoveSingle(ItemB);
			}
			AddItemToInventoryByID(Craft.ProductID);

			ReloadInventory();
		}
	}
}

