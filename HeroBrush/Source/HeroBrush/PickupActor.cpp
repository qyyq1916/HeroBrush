// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"


APickupActor::APickupActor() 
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);

	ItemID = FName("Please enter an ID");
}