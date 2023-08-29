// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HeroCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/ProgressBar.h"
#include "Engine/DataTable.h"
#include "HeroBrushCharacter.generated.h"


UCLASS(config=Game)
class AHeroBrushCharacter : public AHeroCharacter
{
	GENERATED_BODY()

public:

	//variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
		float SmoothBlendTime = 0.75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurEnergy = 100.f;
	

	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
		class UCameraComponent* CameraShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
		class UCameraComponent* CameraThird;
	
	// delegate
	FTimerDelegate HealthDelegate;
	FTimerDelegate EnergyDelegate;
	FTimerDelegate SpeedUpDelegate;


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* MyArms;
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetArms() const { return MyArms; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraThird; }
public:
	AHeroBrushCharacter();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//function
	void ChangeView();
	void ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange);
	void ChangeOnceEnergy(float EnergyRange);
	
	//Interact with other objects(weapons)
	UFUNCTION()
		void OnInteract();

	void SetWeapon(class AWeapon* Weapon);


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
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_PrimaryAttack;

	int AttackAnimSeq = 0; // anim control
	FVector HandLocation; // attack的出发点

	// Burden Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		UAnimMontage* BurdenAnim; // Burden_Attack
	UPROPERTY(EditAnywhere, Category = "Burden_Attack")
		TSubclassOf<AActor> ProjectileClass_Burden;
	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_Burden_Attack;

	// Flash Attack
	UPROPERTY(EditAnywhere, Category = "Flash_Attack")
		TSubclassOf<AActor> ProjectileClass_Flash;
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
		UAnimMontage* AOEAnim; // AOE_Attack
	//UPROPERTY(EditAnywhere, Category = "AOE_Attack")
	//	TSubclassOf<AActor> ProjectileClass_AOE;


	//// 加速事件代理,在有能量消耗的时候使用。
	//FTimerDelegate SpeedUpDelegate;

	//The Weapon Character Armed

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

protected:
	//Basic Attack Func(No Weapon)
	void Primary_Attack();

	void Burden_Attack();

	void Flash_Attack();

	void Flash_TimeElapsed();
	
	void TurnOnSpeed();

	void TurnOffSpeed();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	void CheckTouchActor(AActor* OtherActor);

	void CheckForInteractables();

	void AOE_Attack();
};

//定义数据类型
//
// 融合类型的定义
USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase {
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ComponentID; // 可以融合的物品ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProductID; // 融合之后的物品ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemA; //  是否销毁物品A 物品A就是ComponentID所代表的物品

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemB; //  是否销毁物品B 
};

// 数据表中的类型定义，数据表如果采用了下面结构体的类型，数据表中就会显示他的所有数据，就有点类似于继承。
USTRUCT(BlueprintType)  // 声明为蓝图类型
struct FInventoryItem : public FTableRowBase {
	GENERATED_BODY();

public:

	FInventoryItem() { // 构造函数 变量进行初始化。
		Name = FText::FromString("Item");
		Action = FText::FromString("Use");
		Description = FText::FromString("Please enter a description for this item");
		Value = 10;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;  // 物品的ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APickupActor> Itempickup; // 拾取类型对象

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name; // 对象名字

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action; // 对象作用

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;  // 对象的值

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail; // 储存对象图片信息

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description; // 对该数据的描述

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCraftingInfo> CraftCombinations; // 储存可以相互融合的物品信息

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed; // 是否可以被使用

	bool operator == (const FInventoryItem& Item) const { // 重载等于号，如果他们ID相同，就说明他们两个是属于同一种类型。
		if (ItemID == Item.ItemID) return true;
		else return false;
	}
};
