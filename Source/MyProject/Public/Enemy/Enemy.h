#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"

#include "CharacterEnums.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class MYPROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

// ======================================================================================================================================================
// Class Base
// ======================================================================================================================================================
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;
private:


	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
	AAIController* EnemyController;

// ======================================================================================================================================================
// Attack Section
// ======================================================================================================================================================
public:
	virtual void Attack() override;
	virtual void PlayAttackMontage() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void Die() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

// ======================================================================================================================================================
// AI Section
// ======================================================================================================================================================

public:
	void CheckPatrolTarget();
	void CheckCombatTarget();

protected:
	AActor* ChoosePatrolTarget();
	void MoveToTarget(AActor* Target);
	bool InTargetRange(AActor* Target, double Radius);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

private:
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

// ======================================================================================================================================================
// Animations Section
// ======================================================================================================================================================
protected:

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:


};
