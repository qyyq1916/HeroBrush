// Fill out your copyright notice in the Description page of Project Settings.


#include "FuckerCutter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"
#include "GameplayController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AFuckerCutter::AFuckerCutter() {

}

void AFuckerCutter::BeginPlay()
{
	Super::BeginPlay();
	GetTrancePointsLocation();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
//void AFuckerCutter::EndPlay()
//{
//}
void AFuckerCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckForInteractables();
	if (dead == false) {
		if (CurHealth <= 0) {
			dead = true;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, this->GetActorLocation());
		
		}
	}
}


void AFuckerCutter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFuckerCutter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFuckerCutter::Look);
		PlayerInputComponent->BindAction("Primary_Attack", IE_Pressed, this, &AFuckerCutter::Primary_Attack);
		PlayerInputComponent->BindAction("RAbility", IE_Pressed, this, &AFuckerCutter::RAbility);
		PlayerInputComponent->BindAction("QAbility", IE_Pressed, this, &AFuckerCutter::QAbility);
		PlayerInputComponent->BindAction("CutterFlash", IE_Pressed, this, &AFuckerCutter::CutterFlash);
		//PlayerInputComponent->BindAction("Burden_Attack", IE_Pressed, this, &AFuckerCutter::Burden_Attack);
	}
}


void AFuckerCutter::Primary_Attack() {
	if (CanDoAttack) {
		if (AttackAnimSeq == 0) {
			NowAttackDamage = BaseAttackDamage;//修正造成的伤害数值为普攻伤害数值
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound, this->GetActorLocation());
			PlayAnimMontage(AttackAnim1);
			CanDoAttack = false;
			AttackAnimSeq++;
			AttackAnimSeq = AttackAnimSeq % 4;
		}
		else if (AttackAnimSeq == 1) {
			NowAttackDamage = BaseAttackDamage;
			PlayAnimMontage(AttackAnim2);
			CanDoAttack = false;
			AttackAnimSeq++;
			AttackAnimSeq = AttackAnimSeq % 4;
		}
		else if (AttackAnimSeq == 2) {
			NowAttackDamage = BaseAttackDamage;
			PlayAnimMontage(AttackAnim3);
			CanDoAttack = false;
			AttackAnimSeq++;
			AttackAnimSeq = AttackAnimSeq % 4;
		}
		else if (AttackAnimSeq == 3) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound2, this->GetActorLocation());
			NowAttackDamage = BaseAttackDamage;
			PlayAnimMontage(AttackAnim4);
			CanDoAttack = false;
			AttackAnimSeq++;
			AttackAnimSeq = AttackAnimSeq % 4;
		}
		GetWorldTimerManager().SetTimer(PAttackReset, this, &AFuckerCutter::SetCanDoAttackTrue, AttackSpeed,false);
	}
}
void AFuckerCutter::SetCanDoAttackTrue() {
	CanDoAttack = true;
}
void AFuckerCutter::RAbility() {
	if (CanDoR) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RAttackSound, this->GetActorLocation());
		CanDoR = false;
		ChangeEnergy(false, -1, -20);
		NowAttackDamage = RAttackDamage;
		PlayAnimMontage(RAbilityAnim);
		GetWorldTimerManager().SetTimer(RSkillReset, this, &AFuckerCutter::SetCanDoRTrue, RSkillCD, false);
	}

}
void AFuckerCutter::SetCanDoRTrue() {
	CanDoR = true;
}

void AFuckerCutter::QAbility()
{
	if (CanDoQ) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), QAttackSound, this->GetActorLocation());
		CanDoQ = false;
		ChangeEnergy(false, -1, -20);
		AttackSpeed = 0.3f;
		PlayAnimMontage(QAbilityAnim);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PirAttackSound, this->GetActorLocation());
		GetWorldTimerManager().SetTimer(QSkillReset, this, &AFuckerCutter::SetCanDoQTrue, QSkillCD, false);//cd12秒
		GetWorldTimerManager().SetTimer(QSkillLast, this, &AFuckerCutter::SetAttackSpeedNormal, 6, false);//加攻速6秒
	}
}

void AFuckerCutter::SetCanDoQTrue()
{
	CanDoQ = true;
}
void AFuckerCutter::SetAttackSpeedNormal() {
	AttackSpeed = 1.f;
}

void AFuckerCutter::CutterFlash()
{
	if (CanDoFlash) {
		CanDoFlash = false;
		GetCharacterMovement()->MaxWalkSpeed = 8000.0f;
		GetWorldTimerManager().SetTimer(FlashReset, this, &AFuckerCutter::SetCanDoFlashTrue, FlashCD, false);//cd4秒
		GetWorldTimerManager().SetTimer(FlashLast, this, &AFuckerCutter::SetSpeedNormal, 0.5, false);//加速0.2秒
	}
}
void AFuckerCutter::SetCanDoFlashTrue()
{
	CanDoFlash = true;
}
void AFuckerCutter::SetSpeedNormal()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AFuckerCutter::PrimaryAttack_TimeElapsed()
{
}

void AFuckerCutter::GetTrancePointsLocation()
{
	KnifePointLocation_Array.Empty();
	for (int i = 0; i < KnifePointNames_Array.Num(); i++) {
		KnifePointLocation_Array.Emplace(GetMesh()->GetSocketLocation(KnifePointNames_Array[i]));//获取网格体上特定名字的擦朝的位置，并将其存在KnifePointLocation_Array容器中

	}
}
void AFuckerCutter::MontageWindowBegin()
{
	GetTrancePointsLocation();
	GetWorld()->GetTimerManager().SetTimer(Attacktest, this, &AFuckerCutter::MontageWindowBegin_Delay,0.01, true);//每0.01秒发射一轮射线
}
void AFuckerCutter::MontageWindowEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(Attacktest);
	HittingEnemy = nullptr;
	HittingEnemy_Array.Empty();
}

void AFuckerCutter::MontageWindowBegin_Delay()
{
	for (int i = 0; i < KnifePointNames_Array.Num(); i++) {
		UKismetSystemLibrary::LineTraceMultiForObjects(this, KnifePointLocation_Array[i],GetMesh()->GetSocketLocation(KnifePointNames_Array[i]),ObjectType,false,IgnoreActors_Array,EDrawDebugTrace::ForDuration,HitResult,true); // EDrawDebugTrace::ForDuration
		for (int j = 0; j < HitResult.Num(); j++) {
			HittingEnemy = Cast<AEnemy>(HitResult[j].GetActor());
			if (HittingEnemy) {
				if (!HittingEnemy_Array.Contains(HittingEnemy)) {
					HittingEnemy_Array.AddUnique(HittingEnemy);
					HittingEnemy->ChangeHealth(false, -1, -NowAttackDamage);
					ChangeHealth(false, -1, 0.25*NowAttackDamage);
				}
			}
		}
	}
	GetTrancePointsLocation();
}

void AFuckerCutter::CheckForInteractables() {
	// 射线拾取
	FHitResult CutterHitResult; // 存储碰撞的结果的变量

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector MuzzleOffset;
	MuzzleOffset.Set(100.0f, 50.0f, 20.0f);

	FVector StartTrace = CameraLocation; // 射线起始点
	FVector EndTrace = (FTransform(CameraRotation).TransformVector(MuzzleOffset))*2.f + StartTrace; // 射线终止点。

	FCollisionQueryParams QueryParams; // 储存了碰撞相关的信息
	QueryParams.AddIgnoredActor(this); // 将我们角色自身忽略掉，减少性能开销

	AGameplayController* controller = Cast<AGameplayController>(GetController()); // 玩家控制器

	/*TArray<AActor*> IgnoreActors;
	bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, CutterHitResult, true);
	if (bIsHit)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), CutterHitResult.GetActor()->GetName());
	}*/

	if (GetWorld()->LineTraceSingleByChannel(CutterHitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams) && controller) {
		//检查我们点击的项目是否是一个可交互的项目
		if (AInteractable* Interactable = Cast<AInteractable>(CutterHitResult.GetActor())) {
			controller->CurrentInteractable = Interactable;
			return;
		}
	}

	//如果我们没有击中任何东西，或者我们击中的东西不是一个可交互的，设置currentinteractable为nullptr
	controller->CurrentInteractable = nullptr;

}