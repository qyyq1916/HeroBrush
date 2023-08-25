// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpereComp"));
	RootComponent = SpereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	//Rotate = false;
	//RotationSpeed = 100;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SpereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	SpereComp->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);


}

//OnOverlapBegin to realize touching object
void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ParticleOverlap)  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOverlap, GetActorLocation());
	if (SoundOverlap) UGameplayStatics::PlaySound2D(GetWorld(), SoundOverlap);
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (Rotate) {
		FRotator Rotation = MeshComp->GetComponentRotation();
		Rotation.Yaw += DeltaTime * RotationSpeed;
		MeshComp->SetWorldRotation(Rotation);
	}*/

}

