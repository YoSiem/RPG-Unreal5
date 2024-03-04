// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(OtherActor);

	if (pPlayer)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation( this, PickupSound, GetActorLocation() );
		}

		if (GEngine)
		{
			FString Message = FString::Printf(TEXT("Added %d gold"), Gold);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
		}

		Destroy();

	}

}


