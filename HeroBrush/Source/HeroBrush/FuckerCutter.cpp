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
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AFuckerCutter::AFuckerCutter() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true;

	CameraThird = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroThirdCamera"));
	CameraThird->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraThird->SetRelativeLocation(FVector(20.0f, 100.0f, 100.0f));
	CameraThird->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
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
void AFuckerCutter::EndPlay()
{
}
void AFuckerCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurHealth <= 0) {
		Death();
	}
}
void AFuckerCutter::Death() {
	PlayAnimMontage(DeathAnim);
}
void AFuckerCutter::PlayHurtAnime() {

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
		//PlayerInputComponent->BindAction("Burden_Attack", IE_Pressed, this, &AFuckerCutter::Burden_Attack);
	}
}
void AFuckerCutter::ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange) {
	if (IsLong) {

	}
	else {
		ChangeOnceEnergy(EnergyRange);
	}
}
void AFuckerCutter::ChangeOnceEnergy(float EnergyRange) {
	if (CurEnergy + EnergyRange < TotalEnergy && CurEnergy + EnergyRange > 0)
		CurEnergy += EnergyRange;
	else if (CurEnergy + EnergyRange >= TotalEnergy)
		CurEnergy = TotalEnergy;
	else if (CurEnergy + EnergyRange <= 0)
		CurEnergy = 0.0f;
}

void AFuckerCutter::Primary_Attack() {
	if (CanDoAttack) {
		if (AttackAnimSeq == 0) {
			NowAttackDamage = BaseAttackDamage;//修正造成的伤害数值为普攻伤害数值
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
		CanDoR = false;
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
		CanDoQ = false;
		AttackSpeed = 0.3f;
		PlayAnimMontage(QAbilityAnim);
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
		UKismetSystemLibrary::LineTraceMultiForObjects(this, KnifePointLocation_Array[i],GetMesh()->GetSocketLocation(KnifePointNames_Array[i]),ObjectType,false,IgnoreActors_Array,EDrawDebugTrace::None,HitResult,true); // EDrawDebugTrace::ForDuration
		for (int j = 0; j < HitResult.Num(); j++) {
			HittingEnemy = Cast<AEnemy>(HitResult[j].GetActor());
			if (HittingEnemy) {
				if (!HittingEnemy_Array.Contains(HittingEnemy)) {
					HittingEnemy_Array.AddUnique(HittingEnemy);
					HittingEnemy->ChangeHealth(false, -1, -NowAttackDamage);
				}
			}
		}
	}
	GetTrancePointsLocation();
}
