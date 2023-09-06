// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayController.h"
#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interactable.h"
#include "HeroBrushCharacter.h"
#include "FuckerCutter.h"
#include "GameHeroCharacter.h"

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
		}
	}
}

void AGameplayController::MinusItemToInventoryByID(FName ID, int& OriginQuantity, int& AfterQuantity)
{
	int32 Quantity = 1;

	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	UDataTable* ItemTable = GameMode->GetItemDB();

	FInventoryItem* ItemToMinus = ItemTable->FindRow<FInventoryItem>(ID, "");

	OriginQuantity = 0;

	if (ItemToMinus) {
		// Check if the item already exists in the inventory
		for (FInventoryItem& InventoryItem : Inventory) {
			if (InventoryItem.ItemID == ItemToMinus->ItemID) {
				// Item already exists, decrease the quantity
				OriginQuantity = InventoryItem.Quantity;
				InventoryItem.Quantity -= Quantity;
				AfterQuantity = InventoryItem.Quantity;
				if (AfterQuantity <= 0) {
					Inventory.RemoveSingle(*ItemToMinus);
				}
			}
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
				for (FInventoryItem& InventoryItem : Inventory) {
					if (ItemA.ItemID == InventoryItem.ItemID) {
						InventoryItem.Quantity -= 1;
						if (InventoryItem.Quantity <= 0) {
							Inventory.RemoveSingle(ItemA);
						}
						break;
					}
				}
			}
			if (Craft.bDestroyItemB) {
				for (FInventoryItem& InventoryItem : Inventory) {
					if (ItemB.ItemID == InventoryItem.ItemID) {
						InventoryItem.Quantity -= 1;
						if (InventoryItem.Quantity <= 0) {
							Inventory.RemoveSingle(ItemB);
						}
						break;
					}
				}
			}
			AddItemToInventoryByID(Craft.ProductID);

			ReloadInventory();
		}
	}
}
