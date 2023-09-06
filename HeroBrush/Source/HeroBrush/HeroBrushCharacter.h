// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameHeroCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/ProgressBar.h"
#include "Engine/DataTable.h"
#include "HeroBrushCharacter.generated.h"


UCLASS(config=Game)
class AHeroBrushCharacter : public AGameHeroCharacter
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
		class UCameraComponent* CameraShoot;
public:
	AHeroBrushCharacter();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//function
	void ChangeView();

	
	//Interact with other objects(weapons)
	UFUNCTION()
		void OnInteract();

	void SetWeapon(class AWeapon* Weapon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* PirAttackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* PirAttackSound2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* QAttackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* RAttackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* DeadSound;
	// inline function
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	///** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCameraFirst() const { return CameraFirst; }
	//FORCEINLINE class UCameraComponent* GetFollowCameraThird() const { return CameraThird; }


	//Overlap with other objects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon* ActiveOverlapItem;


protected:

	//Basic Attack(No Weapon)
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim1;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim2;
	UPROPERTY(EditAnywhere, Category = "Primary_Attack")
		UAnimMontage* AttackAnim3;
		
	UFUNCTION()
		void ChangePrimary_Attack_CD();
	UFUNCTION()
		void PlayPrimaryAttackAnim();
	
	bool Primary_Attack_CD = true;
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_PrimaryAttack_CD;

	// 加入特殊状态，霸体状态
	UPROPERTY(EditAnywhere)
		bool isQuickAttack = false;  // 快速攻击和霸体状态
	UPROPERTY(EditAnywhere)
		float QuickAttackTime = 10.0f;
	UPROPERTY(EditAnywhere)
		float QuickAttackCD = 15.0f;

	FTimerHandle QuickAttackLast;
	FTimerHandle QuickAttackReset;
	bool CanDoQuickAttack = true; // 是否可以执行
	void SetCanDoQuickAttackTrue();
	void SetQuickAttackFalse();

	UFUNCTION(BlueprintCallable)
		void ChangeQuickAttack(); // 使用魔药来使用，调用这个函数，持续20s
	UFUNCTION(BlueprintCallable)
		bool GetIsQucikAttack();

	UPROPERTY()
		USkeletalMeshComponent* Skeletal;
	UPROPERTY(EditAnywhere, Category = "Skeleton_Info")
		USkeletalMesh* FirstSeketal;
	UPROPERTY(EditAnywhere, Category = "Skeleton_Info")
		USkeletalMesh* SecondSeketal;
	UFUNCTION()
		void ChangeSkeletonToCool();
	UFUNCTION()
		void ChangeSkeletonToNormal();

	int AttackAnimSeq = 0; // anim control
	FVector HandLocation; // attack的出发点

	// Burden Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		UAnimMontage* BurdenAnim; // Burden_Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		UAnimMontage* BurdenFastAnim; // Burden_Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		TSubclassOf<AActor> ProjectileClass_Burden;

	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_Burden_Attack;

	// Flash Attack
	UPROPERTY(EditAnywhere, Category = "Flash_Attack")
		TSubclassOf<AActor> ProjectileClass_Flash;
	UPROPERTY(EditAnywhere, Category = "Flash_Attack")
		UAnimMontage* Flash_In_Anim;
	UPROPERTY(EditAnywhere, Category = "Flash_Attack")
		UAnimMontage* Flash_Out_Anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash_Attack")
		float FlashDistance = 600.0f;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_Flash_Attack;


	// 技能冷却
	UPROPERTY(EditAnywhere, Category = "Flash_Attack_TimePeriod")
		int Flash_Attack_Time = 180;
	bool isFlashActive = true;

	UPROPERTY(EditAnywhere, Category = "SpeedUp")
		float SetSpeedMax = 1000.0f;

	// AOE
	UPROPERTY(EditAnywhere, Category = "AOE_Attack")
		TSubclassOf<AActor> ProjectileClass_AOE;
	UPROPERTY(EditAnywhere, Category = "AOE_Attack")
		UAnimMontage* AOEAnim; // AOE_Attack


	// 受伤动画
	UPROPERTY(EditAnywhere, Category = "Character_Effect")
		UAnimMontage* HurtAnim;
	// 死亡动画
	UPROPERTY(EditAnywhere, Category = "Character_Effect")
		UAnimMontage* DeathAnim;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle DeathTimer;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class AWeapon* EquiqedWeapon;


public:
	virtual void BeginPlay();
	//virtual void EndPlay(); 想要使用这个销毁定时器
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PrimaryAttack_TimeElapsed();

	UFUNCTION(BlueprintCallable)
	void Burden_Attack_TimeElapsed();

	UFUNCTION(BlueprintCallable)
		void AOE_Attack_TimeElapsed();

	// 加血,动画调用
	UFUNCTION(BlueprintCallable)
		void Recovery();
	void PlayAnimRecovery();
	UPROPERTY(EditAnywhere, Category = "Character_Effect")
		UAnimMontage* RecoveryAnim; // AOE_Attack
protected:
	//Basic Attack Func(No Weapon)
	void Primary_Attack();

	void Burden_Attack();

	void Flash_Attack();

	void Flash_TimeElapsed();
	
	void TurnOnSpeed();

	void TurnOffSpeed();

	//void CheckForInteractables();

	void AOE_Attack();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	//void CheckTouchActor(AActor* OtherActor);
};

////定义数据类型
////
//// 融合类型的定义
//USTRUCT(BlueprintType)
//struct FCraftingInfo : public FTableRowBase {
//	GENERATED_BODY();
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FName ComponentID; // 可以融合的物品ID
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FName ProductID; // 融合之后的物品ID
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		bool bDestroyItemA; //  是否销毁物品A 物品A就是ComponentID所代表的物品
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		bool bDestroyItemB; //  是否销毁物品B 
//};
//
//// 数据表中的类型定义，数据表如果采用了下面结构体的类型，数据表中就会显示他的所有数据，就有点类似于继承。
//USTRUCT(BlueprintType)  // 声明为蓝图类型
//struct FInventoryItem : public FTableRowBase {
//	GENERATED_BODY();
//
//public:
//
//	FInventoryItem() { // 构造函数 变量进行初始化。
//		Name = FText::FromString("Item");
//		Action = FText::FromString("Use");
//		Description = FText::FromString("Please enter a description for this item");
//		Value = 10;
//		Quantity = 0;
//	}
//
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FName ItemID;  // 物品的ID
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TSubclassOf<class APickupActor> Itempickup; // 拾取类型对象
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int32 Quantity;  // 对象的数量
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FText Name; // 对象名字
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FText Action; // 对象作用
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int32 Value;  // 对象的值
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		UTexture2D* Thumbnail; // 储存对象图片信息
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FText Description; // 对该数据的描述
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<FCraftingInfo> CraftCombinations; // 储存可以相互融合的物品信息
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		bool bCanBeUsed; // 是否可以被使用
//
//	bool operator == (const FInventoryItem& Item) const { // 重载等于号，如果他们ID相同，就说明他们两个是属于同一种类型。
//		if (ItemID == Item.ItemID) return true;
//		else return false;
//	}
//};
