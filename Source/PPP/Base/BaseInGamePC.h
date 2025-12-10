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
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character)
	TObjectPtr<ABaseInGameCharacter> MyChara;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Data)
	FVector Destination;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Data)
	uint8 bIsMoving : 1;

protected:
	uint8 bIsLerpMoving : 1;

public:
	UFUNCTION()
	void Follow();

	UFUNCTION()
	bool CheckLand(FVector& ImpactPoint);

	UFUNCTION()
	void SpawnDestinationSystem();

	UFUNCTION(BlueprintCallable)
	void SimpleMoveToLocation(const FVector& GoalLocation);
};
