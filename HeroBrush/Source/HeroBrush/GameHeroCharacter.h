// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroCharacter.h"
#include "Engine/DataTable.h"
#include "GameHeroCharacter.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class HEROBRUSH_API AGameHeroCharacter : public AHeroCharacter
{
	GENERATED_BODY()
public:
	AGameHeroCharacter();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float TotalEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		float CurEnergy = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroCamera");
	class UCameraComponent* CameraThird;

	UFUNCTION(BlueprintCallable)
		bool GetDeathStatus();


	// function
	UFUNCTION(BlueprintCallable)
		void ChangeEnergy(bool IsLong, int TimePeriod, float EnergyRange);
	void ChangeOnceEnergy(float EnergyRange);
	

protected:
	void CheckForInteractables();
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
		Quantity = 0;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;  // 物品的ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APickupActor> Itempickup; // 拾取类型对象

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Quantity;  // 对象的数量

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