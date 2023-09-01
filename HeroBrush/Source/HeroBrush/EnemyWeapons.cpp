// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeapons.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AEnemyWeapons::AEnemyWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KnifeMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	KnifeMeshComponent->SetupAttachment(RootComponent);

	EnemyKnifePointNames_Array = { "EnemyWeaponCao1","EnemyWeaponCao2","EnemyWeaponCao3","EnemyWeaponCao4","EnemyWeaponCao5" };
	EnemyObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
}



// Called when the game starts or when spawned
void AEnemyWeapons::BeginPlay()
{
	Super::BeginPlay();
	GetTrancePointsLocation();
}



// Called every frame
void AEnemyWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemyWeapons::GetTrancePointsLocation()
{
	EnemyKnifePointLocation_Array.Empty();
	for (int i = 0; i < EnemyKnifePointNames_Array.Num(); i++) {
		EnemyKnifePointLocation_Array.Emplace(KnifeMeshComponent->GetSocketLocation(EnemyKnifePointNames_Array[i]));//获取网格体上特定名字的擦朝的位置，并将其存在KnifePointLocation_Array容器中

	}
}
void AEnemyWeapons::MontageWindowBegin()
{
	GetTrancePointsLocation();
	GetWorld()->GetTimerManager().SetTimer(Attacktest, this, &AEnemyWeapons::MontageWindowBegin_Delay, 0.01, true);//每0.01秒发射一轮射线
}

void AEnemyWeapons::MontageWindowEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(Attacktest);
	HittingCharacter = nullptr;
	HittingCharacter_Array.Empty();
}

void AEnemyWeapons::MontageWindowBegin_Delay()
{
	for (int i = 0; i < EnemyKnifePointNames_Array.Num(); i++) {
		UKismetSystemLibrary::LineTraceMultiForObjects(this, EnemyKnifePointLocation_Array[i], KnifeMeshComponent->GetSocketLocation(EnemyKnifePointNames_Array[i]), EnemyObjectType, false, EnemyIgnoreActors_Array, EDrawDebugTrace::ForDuration, EnemyHitResult, true); // EDrawDebugTrace::ForDuration
		for (int j = 0; j < EnemyHitResult.Num(); j++) {
			HittingCharacter = Cast<AHeroCharacter>(EnemyHitResult[j].GetActor());
			if (HittingCharacter) {
				if (!HittingCharacter_Array.Contains(HittingCharacter)) {
					HittingCharacter_Array.AddUnique(HittingCharacter);
					HittingCharacter->ChangeHealth(false, -1, -10);
				}
			}
		}
	}
	GetTrancePointsLocation();
}