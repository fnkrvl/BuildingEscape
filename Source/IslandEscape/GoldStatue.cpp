// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldStatue.h"
#include "Otolith.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AGoldStatue::AGoldStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComp;
	BoxComp->InitBoxExtent(FVector(7.0f, 7.0f, 12.0f));
	BoxComp->SetCollisionProfileName(TEXT("Statue"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MeshComp->SetupAttachment(RootComponent);

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.5, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGoldStatue::OnOverlapBegin);

	RotSpeed = FVector(0.0f, 0.0f, 7.0f);
	Distance = 35.f;
	Time = 0.0f;
}

// Called when the game starts or when spawned
void AGoldStatue::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGoldStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat Rot = GetActorQuat();
	const FQuat YawRotation(FVector::UpVector, RotSpeed.Z * DeltaTime);

	Rot *= YawRotation;

	SetActorRotation(Rot);

	FVector Pos = GetActorLocation();
	const float DeltaHeight = FMath::Sin(5.0f * (Time + DeltaTime)) - FMath::Sin(5.0f * Time );
	Pos.Z += DeltaHeight * Distance;
	SetActorLocation(Pos);

	Time += DeltaTime;
}

void AGoldStatue::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOtolith* Otolith = Cast<AOtolith>(OtherActor);
	if (Otolith != nullptr)
		Destroy();
}
