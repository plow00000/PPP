// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseInGamePC.generated.h"

class ABaseInGameCharacter;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PPP_API ABaseInGamePC : public APlayerController
{
	GENERATED_BODY()

public:
	ABaseInGamePC();

protected:
	virtual void OnPossess(APawn* PawnToPossess) override;

	virtual void BeginPlay() override;

public:
	virtual void PlayerTick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInputMappingContext> TestIMC;
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character)
	TObjectPtr<ABaseInGameCharacter> MyChara;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	FVector Destination;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	uint8 bIsMoving : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Action)
	uint8 bOnAction : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	uint8 bCanCombo : 1;
protected:

public:
	UFUNCTION()
	void Follow();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckLand(FVector& ImpactPoint);

	UFUNCTION()
	void SpawnDestinationSystem();

	void ChangeKeyMapping();

	UFUNCTION(BlueprintCallable)
	void TurnCharacterToLookCursor();

};
