// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
class ATreasure;

UCLASS()
class MYPROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void SpawnDrop();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:	

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category = "Drop")
	TArray< TSubclassOf<ATreasure>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Drop")
	int8 minDropCount = 1;

	UPROPERTY(EditAnywhere, Category = "Drop")
	int8 maxDropCount = 1;

	UPROPERTY(EditAnywhere, Category = "Drop")
	float SpawnRadius = 100.f;

	bool bHasDropped = false;


};
