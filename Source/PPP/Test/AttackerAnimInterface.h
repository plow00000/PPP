// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackerAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackerAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PPP_API IAttackerAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CheckCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCheckingCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopCheckingCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MontageEnd();
};
