// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h" 
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABreakableActor::ABreakableActor()
{

	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	SpawnDrop();

}

void ABreakableActor::SpawnDrop()
{
	int8 ItemCount = FMath::RandRange(minDropCount, maxDropCount);
	UWorld* World = GetWorld();

	if (World && TreasureClasses.Num() > 0 && !bHasDropped)
	{
		bHasDropped = true;

		for (int32 i = 0; i < ItemCount; ++i)
		{
			int32 SelectedIndex = FMath::RandRange(0, TreasureClasses.Num() - 1);
			FVector Location = GetActorLocation();
			Location.Z += 5.f;
			Location.Y += FMath::RandRange(-SpawnRadius, SpawnRadius);
			Location.X += FMath::RandRange(-SpawnRadius, SpawnRadius);

			World->SpawnActor<ATreasure>(TreasureClasses[SelectedIndex], Location, GetActorRotation());

		}

	}
}



