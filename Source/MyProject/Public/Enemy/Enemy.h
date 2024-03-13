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

public:
	AEnemy();
	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/* </IHitInterface> */

protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void Die() override;
	virtual int32 PlayDeathMontage() override;
	/* </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:

	/* State Checks */
	bool IsDead();
	bool IsEngaged();
	bool IsAttacking();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsOutsideAttackRadius();
	bool IsOutsideCombatRadius();
	bool InTargetRange(AActor* Target, double Radius);

	/* Target Selection and Movement */
	AActor* ChoosePatrolTarget();
	void MoveToTarget(AActor* Target);

	/* AI Behavior Triggers */
	void StartPatrolling();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void ClearPatrolTimer();
	void ChaseTarget();
	void CheckCombatTarget();
	void StartAttackTimer();
	void ClearAttackTimer();
	void SpawnDefaultWeapon();
	void InitializeEnemy();

	/* State Management */
	void ShowHealthBar();
	void HideHealthBar();
	void LoseInterest();


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);	// Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 150.f;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

};
