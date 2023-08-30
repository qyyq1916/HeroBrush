// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "BaseWeaponBullets.h"
#include "AOEItem.h"
// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	 PrimaryActorTick.bCanEverTick = true;
	 StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	 StatusWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//auto InfoWidegetClass1 = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/Content/ThirdPerson/Widgets/EnemyStatus.EnemyStatus_C'"));
	//StatusWidgetComponent->SetWidgetClass(InfoWidegetClass1);
	////StatusWidgetComponent->GetChildComponent
	//StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//StatusWidgetComponent->SetPivot(FVector2D(1, 1));
	//StatusWidgetComponent->SetDrawSize(FVector2D(120.0f, 10.0f));
	//StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (CurHealth <= 0)
	{
		PlayAnimMontage(DeathAnim);
		GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::PostDeadAnim, 1.0f);
	}*/
}

void AEnemy::PostDeadAnim()
{
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
	Destroy();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	CheckTouchActor(OtherActor);
}
void AEnemy::CheckTouchActor(AActor* OtherActor)
{
	auto actor = Cast<ABaseWeaponBullets>(OtherActor);
	// 如果是来自英雄的bullets
	if (actor != nullptr && actor->DamageFrom) {
		if (actor->BulletsInfo == 0) {
			ChangeHealth(false, -1, actor->BulletDamage); // 第一类的掉血
			
		}
		else if (actor->BulletsInfo == 1) {
			ChangeHealth(false, -1, actor->BulletDamage);
		}
		if (CurHealth > 0 )
			PlayAnimMontage(HurtAnim);
		else {
			PlayAnimMontage(DeathAnim);
			GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::PostDeadAnim, 0.5f);
		}
	}

	// aoe的来源
	auto actor1 = Cast<AAOEItem>(OtherActor);
	if (actor1 != nullptr && actor1->DamageFrom) {
		if (actor1->AOEInfo == 0) {

			ChangeHealth(true, 1, actor1->AOEDamage);

		}
		PlayAnimMontage(DeathAnim);
	}
}
void AEnemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	CheckOffActor(OtherActor);
}
void AEnemy::CheckOffActor(AActor* OtherActor)
{
	// aoe的来源
	auto actor1 = Cast<AAOEItem>(OtherActor);
	if (actor1 != nullptr && actor1->DamageFrom) {
		if (actor1->AOEInfo == 0) {
			GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
			HealthDelegate.Unbind();
		}
	}
	
}
