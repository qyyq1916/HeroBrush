// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeroBrushCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeaponBullets.h"
#include "Components/TextRenderComponent.h"
#include "Item.h"
#include "Weapon.h"
#include "GameplayController.h"
#include "Interactable.h"
#include "Enemy.h"
#include "AOEItem.h"
//////////////////////////////////////////////////////////////////////////
// AHeroBrushCharacter

AHeroBrushCharacter::AHeroBrushCharacter()
{

	CameraShoot = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroShootCamera"));
	CameraShoot->SetupAttachment(CameraBoom);
	CameraShoot->SetRelativeLocation(FVector(30.0f, 80.0f, 80.0f));
	AEnemy::EnemyQuantity = 0;
}

void AHeroBrushCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	PostInitializeComponents();
}




//void AHeroBrushCharacter::EndPlay()
//{
//	Super::EndPlay();
//	
//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Flash_Attack);
//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Flash_waitTime);
//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
//
//}

//////////////////////////////////////////////////////////////////////////
// Input

void AHeroBrushCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroBrushCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeroBrushCharacter::Look);

		//changeView
		PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &AHeroBrushCharacter::ChangeView);
		PlayerInputComponent->BindAction("Primary_Attack", IE_Pressed, this, &AHeroBrushCharacter::Primary_Attack);
		PlayerInputComponent->BindAction("Burden_Attack", IE_Pressed, this, &AHeroBrushCharacter::Burden_Attack);
		PlayerInputComponent->BindAction("Flash_Attack", IE_Pressed, this, &AHeroBrushCharacter::Flash_Attack);
		PlayerInputComponent->BindAction("TurnOnSpeed", IE_Pressed, this, &AHeroBrushCharacter::TurnOnSpeed);
		PlayerInputComponent->BindAction("TurnOffSpeed", IE_Released, this, &AHeroBrushCharacter::TurnOffSpeed);
		
		//Interact
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AHeroBrushCharacter::OnInteract);

		PlayerInputComponent->BindAction("AOE_Attack", IE_Pressed, this, &AHeroBrushCharacter::AOE_Attack);

		PlayerInputComponent->BindAction("ChangeQuickAttack", IE_Pressed, this, &AHeroBrushCharacter::ChangeQuickAttack);
		
		PlayerInputComponent->BindAction("PlayAnimRecovery", IE_Pressed, this, &AHeroBrushCharacter::PlayAnimRecovery);

	}

}

void AHeroBrushCharacter::OnInteract() 
{
	AWeapon* Weapon = Cast<AWeapon>(ActiveOverlapItem);
	if (Weapon) {
		Weapon->Equip(this);
		ActiveOverlapItem = nullptr;
	}

}

void AHeroBrushCharacter::SetWeapon(AWeapon* Weapon)
{
	if (EquiqedWeapon) {
		EquiqedWeapon->Destroy();
	}
	EquiqedWeapon = Weapon;
	Weapon->TextCue->SetVisibility(false);

}


void AHeroBrushCharacter::ChangeView()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (CameraThird->IsActive()) {
		CameraBoom->TargetArmLength = 200.0f;
		CameraShoot->SetActive(true);

		CameraThird->SetActive(false);
		
	}
	else if (CameraShoot->IsActive()) {
		CameraBoom->TargetArmLength = 400.0f;
		CameraThird->SetActive(true);

		CameraShoot->SetActive(false);
	}
	else {
		CameraBoom->TargetArmLength = 400.0f;
		CameraThird->SetActive(true);	
		CameraShoot->SetActive(false);
	}
}

//Basic Attack
void AHeroBrushCharacter::Primary_Attack() {
	if (Primary_Attack_CD && !isQuickAttack) {
		Primary_Attack_CD = false;
		PlayPrimaryAttackAnim();
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack_CD, this, &AHeroBrushCharacter::ChangePrimary_Attack_CD, 0.5f);
	}
	else if (isQuickAttack) {
		PlayPrimaryAttackAnim();
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AHeroBrushCharacter::PrimaryAttack_TimeElapsed, 0.1f);
	}
		
	// 非加速攻击在动画蓝图中。
}
void AHeroBrushCharacter::ChangePrimary_Attack_CD()
{
	Primary_Attack_CD = true;
}

void AHeroBrushCharacter::PlayPrimaryAttackAnim()
{
	if (AttackAnimSeq == 0) {
		PlayAnimMontage(AttackAnim1);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound, this->GetActorLocation());
		HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		AttackAnimSeq++;
		AttackAnimSeq = AttackAnimSeq % 3;
	}
	else if (AttackAnimSeq == 1) {
		PlayAnimMontage(AttackAnim2);
		HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
		AttackAnimSeq++;
		AttackAnimSeq = AttackAnimSeq % 3;
	}
	else if (AttackAnimSeq == 2) {
		PlayAnimMontage(AttackAnim3);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound2, this->GetActorLocation());
		HandLocation = GetMesh()->GetSocketLocation("Muzzle_03");
		AttackAnimSeq++;
		AttackAnimSeq = AttackAnimSeq % 3;
	}
}
// you can set shoot location and shoot speed
void AHeroBrushCharacter::PrimaryAttack_TimeElapsed() {

	FromLocation = HandLocation; // 设置开始的rotation
	GetAimRotation(); // 得到rotation

	const FTransform SpawnTM = FTransform(AimRotation, FromLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams); // 使用已有的蓝图加载，在蓝图中设置
}



void AHeroBrushCharacter::Burden_Attack()
{
	if (CurEnergy >= 5.0f) {
		if (!isQuickAttack)
			
			PlayAnimMontage(BurdenAnim);
		else
			PlayAnimMontage(BurdenFastAnim);
			
		HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	}
	

	//GetWorldTimerManager().SetTimer(TimerHandle_Burden_Attack, this, &AHeroBrushCharacter::Burden_Attack_TimeElapsed, 1.0f);
}

void AHeroBrushCharacter::Burden_Attack_TimeElapsed() {

	FromLocation = HandLocation; // 设置开始的rotation

	const FTransform SpawnTM = FTransform(this->GetActorRotation(), FromLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass_Burden, SpawnTM, SpawnParams); // 使用已有的蓝图加载，在蓝图中设置
	ChangeEnergy(false, -1, -5.0f);
}


void AHeroBrushCharacter::Flash_Attack()
{
	//if (isFlashActive)
	{
		isFlashActive = false;
		FVector PresentLocation = this->GetActorLocation();
		FVector ForwardLocation = GetActorForwardVector();
		FVector NextLocation = PresentLocation + ForwardLocation * FlashDistance;

		FromLocation = PresentLocation;
		AimLocation = NextLocation;

		FHitResult HitResult; // 存储碰撞的结果的变量

		FVector StartTrace = GetActorLocation(); // 射线起始点
		FVector EndTrace = NextLocation; // 射线终止点。
		//FCollisionQueryParams QueryParams; // 储存了碰撞相关的信息
		//QueryParams.AddIgnoredActor(this); // 将我们角色自身忽略掉，减少性能开销

		TArray<AActor*> IgnoreActors;

		//bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);
		bool bIsHit = UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartTrace, EndTrace, TEXT("Flash"), false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

		FVector RealLocation = NextLocation;
		if (bIsHit)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), HitResult.GetActor()->GetName());
			RealLocation = HitResult.ImpactPoint - ForwardLocation * 30.0f;
		}

		if (CurEnergy >= 5.0f) {
			GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AHeroBrushCharacter::Flash_TimeElapsed, 0.2f);
			PlayAnimMontage(Flash_In_Anim);
			this->SetActorLocation(RealLocation, false);
			PlayAnimMontage(Flash_Out_Anim);
		}
		
	}
}

void AHeroBrushCharacter::Flash_TimeElapsed() {

	
	GetAimRotation(); // 得到rotation

	const FTransform SpawnTM = FTransform(AimRotation, FromLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass_Flash, SpawnTM, SpawnParams); // 使用已有的蓝图加载，在蓝图中设置
	ChangeEnergy(false, -1, -10.0f);
}


void AHeroBrushCharacter::AOE_Attack()
{
	if (CurEnergy >= 30)
		PlayAnimMontage(AOEAnim);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RAttackSound, this->GetActorLocation());
}
void AHeroBrushCharacter::AOE_Attack_TimeElapsed()
{
	const FTransform SpawnTM = FTransform(GetActorRotation(), GetActorLocation());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ProjectileClass_AOE, SpawnTM, SpawnParams); // 使用已有的蓝图加载，在蓝图中设置
	
	//UE_LOG(LogTemp, Warning, TEXT("AAOEItemLOC_During:%f,%f,%f"), tempAoe->GetActorLocation().X, tempAoe->GetActorLocation().Y, tempAoe->GetActorLocation().Z);
	ChangeEnergy(false, -1, -30.0f);
}

void AHeroBrushCharacter::SetQuickAttackFalse()
{
	isQuickAttack = false;
	GetWorldTimerManager().ClearTimer(QuickAttackLast);
	ChangeSkeletonToNormal();
}
void AHeroBrushCharacter::SetCanDoQuickAttackTrue()
{
	CanDoQuickAttack = true;
	GetWorldTimerManager().ClearTimer(QuickAttackReset);
}

void AHeroBrushCharacter::ChangeQuickAttack()
{
	if (CanDoQuickAttack) {
		CanDoQuickAttack = false;
		isQuickAttack = true;
		ChangeEnergy(false, -1, -20);
		ChangeSkeletonToCool();
		// cd
		GetWorldTimerManager().SetTimer(QuickAttackReset, this, &AHeroBrushCharacter::SetCanDoQuickAttackTrue, QuickAttackCD); // 设置在cd秒后执行true，可以进入这个判断
		// last time
		GetWorldTimerManager().SetTimer(QuickAttackLast, this, &AHeroBrushCharacter::SetQuickAttackFalse, QuickAttackTime); // 持续时间之后将isquickattack设置回false

	}
}

void AHeroBrushCharacter::ChangeSkeletonToCool()
{
	Skeletal = GetMesh();

	if (Skeletal) {
		if (SecondSeketal) 
		{
			Skeletal->SetSkeletalMesh(SecondSeketal);
			// Skeletal->SetRelativeLocation(FVector(0,0,-90));
			// Skeletal->SetRelativeRotation(FRotator(0,0,270));
			Skeletal->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}
	}	
}

void AHeroBrushCharacter::ChangeSkeletonToNormal()
{
	Skeletal = GetMesh();
	if (Skeletal) {
		if (SecondSeketal) 
		{
			Skeletal->SetSkeletalMesh(FirstSeketal);
			// Skeletal->SetRelativeLocation(FVector(0,0,-90));
			// Skeletal->SetRelativeRotation(FRotator(0,0,270));
			Skeletal->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}
	}	
}

void AHeroBrushCharacter::PlayAnimRecovery()
{
	float timeperiod = PlayAnimMontage(RecoveryAnim);
	
}

void AHeroBrushCharacter::Recovery()
{
	ChangeHealth(false, -1, 30.0f);
	ChangeEnergy(false, -1, 30.0f);
}

void AHeroBrushCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 如果已经被使用进入冷却，记录时间，直到time=0，则技能继续可以被使用，不会进入冷却状态。
	if (!isFlashActive) {
		Flash_Attack_Time--;
	}
	if (Flash_Attack_Time <= 0) {
		isFlashActive = true;
		Flash_Attack_Time = 120;
	}
	
	CheckForInteractables();

}

bool AHeroBrushCharacter::GetIsQucikAttack()
{
	return isQuickAttack;
}

void AHeroBrushCharacter::TurnOnSpeed()
{
	//SpeedUpDelegate.BindUObject(this, &AHealthCharacter::ChangeOnceEnergy, -0.5f);
	GetCharacterMovement()->MaxWalkSpeed = SetSpeedMax;

	//GetWorld()->GetTimerManager().SetTimer(SpeedUpTimer, SpeedUpDelegate, 0.05f, true, -1);

}
void AHeroBrushCharacter::TurnOffSpeed()
{
	//SpeedUpDelegate.Unbind();
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//GetWorld()->GetTimerManager().ClearTimer(SpeedUpTimer);
}

void AHeroBrushCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//CheckTouchActor(OtherActor);
	
}
//void AHeroBrushCharacter::CheckTouchActor(AActor* OtherActor)
//{
//	// 留给后面需要接触的时候使用
//	auto actor1 = Cast<AAOEItem>(OtherActor);
//	if(actor1 != nullptr) {
//		
//	}
//		
//}

