// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "BrainComponent.h"
#include "BaseWeaponBullets.h"
#include "EnemyWeapons.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/ProgressBar.h"
#include "AOEItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values

FName AEnemy::WeaponSlot(TEXT("handknife"));

AEnemy::AEnemy()
{
	
	EnemyClearWindows = nullptr;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	 PrimaryActorTick.bCanEverTick = true;
	 StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	 StatusWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	 
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	EnemyQuantity++;
	EnemyTotal = EnemyQuantity;
	Super::BeginPlay();
	auto InfoWidegetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/ThirdPerson/Widgets/EnemyStatus.EnemyStatus_C'"));
	StatusWidgetComponent->SetWidgetClass(InfoWidegetClass);
	StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 95.0f));
	MyCharacterController = GetWorld()->GetFirstPlayerController();


	if (IsNear) {
		SpawnKnife();
	}
}
void AEnemy::RefreshHeadInfo()
{
	UUserWidget* InfoWidget = StatusWidgetComponent->GetUserWidgetObject();
	if (InfoWidget) {
		auto ProgressBar = Cast<UProgressBar>(InfoWidget->GetWidgetFromName(TEXT("AIHealth")));
		if (ProgressBar)
			ProgressBar->SetPercent(CurHealth / TotalHealth);
	}
}
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RefreshHeadInfo();
	SetStatusRotation();
	if (CurHealth <= 0) {
		if (IsFirstTime) {
			IsFirstTime = false;
			EnemyKnife->Destroy();
			EnemyQuantity--;
			if (EnemyQuantity == 0) {
				UClass* EnemyClearWindowsClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/ThirdPerson/Widgets/ClearWindows.ClearWindows_C"));
				if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
					EnemyClearWindows = CreateWidget(PC, EnemyClearWindowsClass);
					if (EnemyClearWindows) {
						EnemyClearWindows->AddToViewport();
					}
				}
			}
		}
	}



}


// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//CheckTouchActor(OtherActor);
	if (CurHealth <= 0) {
		//Destroy();
		//stop BT 
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			AIC->GetBrainComponent()->StopLogic("dead");
		}
		//ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Rogdoll");
		//set lifespan
		SetLifeSpan(3.0f);
	}
}

void AEnemy::SetTarget(AActor* NewTarget)
{
	if (NewTarget)
	{
		Target = NewTarget;
	}
}

void AEnemy::RemoteAttack()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound, this->GetActorLocation());
	FVector BowLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Direction = Target->GetActorLocation() - BowLocation;

	FRotator BowRotation = Direction.Rotation();

	FActorSpawnParameters params;

	params.Instigator = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ensure(ProjectileClass);

	GetWorld()->SpawnActor<AActor>(ProjectileClass, BowLocation, BowRotation);
}

void AEnemy::AoeAttack()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound2, this->GetActorLocation());
	FVector BowLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Direction = Target->GetActorLocation() - BowLocation;

	FRotator BowRotation = Direction.Rotation();

	FActorSpawnParameters params;

	params.Instigator = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ensure(AOEItem);
	GetWorld()->SpawnActor<AActor>(AOEItem, BowLocation, BowRotation);
}
void AEnemy::SpawnKnife() {
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	EnemyKnife = GetWorld()->SpawnActor<AEnemyWeapons>(SpawnLocation, SpawnRotation);//生成敌人武器
	
	if (EnemyKnife)
	{
		//EnemyKnife->KnifeMesh = EnemyKnifeMesh;
		EnemyKnife->KnifeMeshComponent->SetSkeletalMeshAsset(EnemyKnifeMesh);//设置武器的网格体
		EnemyKnife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "handknife");//将刀绑到敌人手上
		//FRotator InstanceRotation = FRotator(-99.84f, -1.728f, -90.f);
		//FVector InstanceVector = FVector(-0.019f, 7.60f, 0.2295f);
		//EnemyKnife->AddActorLocalRotation(InstanceRotation);
		//EnemyKnife->AddActorLocalTransform(FTransform(InstanceVector));
		/*const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName("handknife");
		if (Socket)
		{
			Socket->AttachActor(EnemyKnife, GetMesh());
		}*/
	}
	
	
}

void AEnemy::SetStatusRotation()
{
	if (MyCharacterController != nullptr) {
		ACharacter* MyCharacter = Cast<ACharacter>(MyCharacterController->GetPawn());
		PlayerCameraManager = MyCharacterController->PlayerCameraManager;
		
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FVector SceneLocation = StatusWidgetComponent->GetComponentLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SceneLocation, CameraLocation);
		StatusWidgetComponent->SetWorldRotation(LookAtRotation);
		
		/*TArray<AActor*> IgnoreActors;
		FHitResult HitResult;
		bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), SceneLocation, CameraLocation, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);
		*/
	}
}
