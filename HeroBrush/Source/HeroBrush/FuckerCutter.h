// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "Enemy.h"
#include "FuckerCutter.generated.h"

/**
 * 
 */
UCLASS()
class HEROBRUSH_API AFuckerCutter : public AHeroCharacter
{
	GENERATED_BODY()

public:
	// Fill out your copyright notice in the Description page of Project Settings.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurEnergy = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
	class UCameraComponent* CameraThird;
public:
	AFuckerCutter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	virtual void EndPlay();// 想要使用这个销毁定时器
	virtual void Tick(float DeltaTime) override;
	
	void ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange);
	void ChangeOnceEnergy(float EnergyRange);
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "TArray")
		TArray<FName> KnifePointNames_Array;//需要获取位置的插槽的名字的容器
	TArray<FVector> KnifePointLocation_Array;//刀上插槽位置的容器

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TArray")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	TArray<AActor*> IgnoreActors_Array;
	TArray<FHitResult> HitResult;
	void  MontageWindowBegin();
	void  MontageWindowEnd();
	void  MontageWindowBegin_Delay();
protected:
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim1;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim2;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim3;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim4;
	UPROPERTY(EditAnywhere, Category = "Skill")
		UAnimMontage* RAbilityAnim;
	UPROPERTY(EditAnywhere, Category = "Skill")
		UAnimMontage* QAbilityAnim;
	UPROPERTY(EditAnywhere, Category = "Death")
		UAnimMontage* DeathAnim;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_PrimaryAttack;
	int AttackAnimSeq = 0;

	bool CanDoAttack=true;//判断是否可以普工
	void SetCanDoAttackTrue();//设置普工布尔值回正
	float AttackSpeed=1.0f;//普攻速度
	FTimerHandle PAttackReset;//普攻回正计时器

	void Primary_Attack();


	void PrimaryAttack_TimeElapsed();
	UPROPERTY(BlueprintReadOnly, Category = "Man")
		TArray<AEnemy*>HittingEnemy_Array;
	AEnemy* HittingEnemy = nullptr;
	void GetTrancePointsLocation();//获取插槽位置信息
	FTimerHandle Attacktest;

	void RAbility();
	bool CanDoR=true;
	void SetCanDoRTrue();
	float RSkillCD=8.f;
	FTimerHandle RSkillReset;
	
	void QAbility();
	bool CanDoQ = true;
	void SetCanDoQTrue();
	float QSkillCD = 12.f;
	FTimerHandle QSkillReset;
	void SetAttackSpeedNormal();
	FTimerHandle QSkillLast;


	float NowAttackDamage = 20.f;
	float BaseAttackDamage=20.f;
	float RAttackDamage=100.f;

	void Death();
	void PlayHurtAnime();
};
