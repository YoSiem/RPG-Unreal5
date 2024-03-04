// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (EchoCharacter)
	{
		EchoCharacterMovement = EchoCharacter->GetCharacterMovement();
	}
	

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (EchoCharacterMovement)
	{
		
		GroundSpeed = UKismetMathLibrary::VSizeXY(EchoCharacterMovement->Velocity);
		IsFalling = EchoCharacterMovement->IsFalling();
		CharacterState = EchoCharacter->GetCharacterState();
	}
}
