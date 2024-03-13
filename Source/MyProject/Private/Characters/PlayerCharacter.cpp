// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");



}



void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("SlashCharacter"));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		//EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}

}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);


}

void APlayerCharacter::EKeyPressed(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		CharacterState = static_cast<ECharacterState>(static_cast<int>(OverlappingWeapon->Weapon_Type));
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;

		}
	}
}

void APlayerCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}

}

void APlayerCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		const int32 WeaponType = static_cast<int32>(EquippedWeapon->Weapon_Type);	//can be 1 or 2
		int32 SelectionRangeEnd = (WeaponType == 2) ? 2 : 4;
		const int32 Selection = FMath::RandRange(1, SelectionRangeEnd);

		AnimInstance->Montage_Play(AttackMontage);
		FName SelectionName = FName();

		if (WeaponType == 1) // One-handed weapon
		{
			switch (Selection)
			{
			case 1: SelectionName = FName("1H-Attack1"); break;
			case 2: SelectionName = FName("1H_Attack2"); break;
			case 3: SelectionName = FName("1H_Attack3"); break;
			case 4: SelectionName = FName("1H-Attack4"); break;
			default: break;
			}
		}
		else if (WeaponType == 2) // Two-handed weapon
		{
			switch (Selection)
			{
			case 1: SelectionName = FName("2H-Attack1"); break;
			case 2: SelectionName = FName("2H-Attack2"); break;
			default: break;
			}
		}
		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::CanAttack()
{
	const bool bCanAttack = ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
	
	return bCanAttack;
}

void APlayerCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool APlayerCharacter::CanDisarm()
{
	bool bCanDisarm = ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
	return bCanDisarm;
}

bool APlayerCharacter::CanArm()
{
	bool bCanArm = ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
	return bCanArm;
}

void APlayerCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void APlayerCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void APlayerCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

