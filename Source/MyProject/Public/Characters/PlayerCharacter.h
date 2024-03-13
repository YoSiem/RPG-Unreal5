#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterEnums.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;


UCLASS()
class MYPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
    APlayerCharacter();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    /* Lifecycle */
    virtual void BeginPlay() override;

    /* Input Callbacks */
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void EKeyPressed(const FInputActionValue& Value);
    virtual void Attack() override;
    virtual void AttackEnd() override;
    virtual bool CanAttack() override;

    /* Combat Functions */
    bool CanDisarm();
    bool CanArm();
    void Disarm();
    void Arm();
    void PlayEquipMontage(const FName& SectionName);
    void EquipWeapon(AWeapon* Weapon);
    UFUNCTION(BlueprintCallable)
    void AttachWeaponToBack();
    UFUNCTION(BlueprintCallable)
    void AttachWeaponToHand();
    UFUNCTION(BlueprintCallable)
    void FinishEquipping();

    /* Input Properties */
    UPROPERTY(EditAnywhere, Category = Input)
    UInputMappingContext* CharacterMappingContext;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* MovementAction;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* LookAction;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* JumpAction;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* EKeyAction;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* AttackAction;
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* DodgeAction;

private:
    /* Character Components */
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* CameraBoom;
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* ViewCamera;
    UPROPERTY(VisibleAnywhere, Category = Hair)
    UGroomComponent* Hair;
    UPROPERTY(VisibleAnywhere, Category = Hair)
    UGroomComponent* Eyebrows;
    UPROPERTY(VisibleInstanceOnly)
    AItem* OverlappingItem;
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* EquipMontage;

    /* State Properties */
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;


public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
