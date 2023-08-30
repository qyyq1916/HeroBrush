// Fill out your copyright notice in the Description page of Project Settings.


#include "AOEItem.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h" 
// Sets default values
AAOEItem::AAOEItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("AOEComp"));
	CollisionComp->InitSphereRadius(200.0f);
	/*CollisionComp->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));*/
	CollisionComp->SetCollisionProfileName("UI");

	RootComponent = CollisionComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AOEEffectComp"));
	EffectComp->SetupAttachment(CollisionComp);
	
	//static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleCompAsset(TEXT("ParticleSystem'/Game/GabrielAguiarProd/AoE_MagicAbilities/Particles/P_AoE_Column01_Purple.P_AoE_Column01_Purple'"));
	// //选择加载的粒子效果
	//EffectComp->SetTemplate(ParticleCompAsset.Object);
	InitialLifeSpan = 3.0f; // 默认的时间长度；
}

// Called when the game starts or when spawned
void AAOEItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAOEItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

