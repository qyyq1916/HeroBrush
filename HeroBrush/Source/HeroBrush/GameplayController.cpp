// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayController.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interactable.h"
#include "HeroBrushCharacter.h"

void AGameplayController::AddItemToInventoryByID(FName ID)
{
	int32 Quantity = 1;

	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	UDataTable* ItemTable = GameMode->GetItemDB();
	
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	//UE_LOG(LogTemp, Warning, TEXT("ID:%s"), *ID.ToString());
	if (ItemToAdd) {
		bool bItemFound = false;

		// Check if the item already exists in the inventory
		for (FInventoryItem& InventoryItem : Inventory) {
			if (InventoryItem.ItemID == ItemToAdd->ItemID) {
				// Item already exists, increase the quantity
				InventoryItem.Quantity += Quantity;
				bItemFound = true;
				break;
			}
		}

		// If the item doesn't exist in inventory, add it
		if (!bItemFound) {
			Inventory.Add(*ItemToAdd);
			//FInventoryItem& InventoryItem = new FInventoryItem();
			//InventoryItem.Quantity += Quantity;

		}
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

