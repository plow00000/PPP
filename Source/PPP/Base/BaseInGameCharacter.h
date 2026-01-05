// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Test/AttackerAnimInterface.h"
#include "BaseInGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABaseInGamePC;

UCLASS()
class PPP_API ABaseInGameCharacter : public ACharacter, public IAttackerAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseInGameCharacter();

public:
	virtual void CheckCombo_Implementation() override;

	inline virtual void StartCheckingCombo_Implementation() override;

	inline virtual void StopCheckingCombo_Implementation() override;

	inline virtual void MontageEnd_Implementation() override;

private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	inline USpringArmComponent* GetSpringArm() { return SpringArm; }
	inline UCameraComponent* GetCamera() { return Camera; }

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	TObjectPtr<UAnimMontage> BasicAttack;

	TObjectPtr<ABaseInGamePC> MyPC;
};