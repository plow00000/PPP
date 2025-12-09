// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInGamePC.h"
#include "BaseInGameCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"

ABaseInGamePC::ABaseInGamePC()
{
	bShowMouseCursor = true;
}

void ABaseInGamePC::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI F;
	F.SetHideCursorDuringCapture(false);

	SetInputMode(F);

	if (!IsLocalPlayerController())
	{
		return;
	}
	ULocalPlayer* LP = Cast<ULocalPlayer>(Player);
	if (LP)
	{
		UEnhancedInputLocalPlayerSubsystem* EISubSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (IMC)
		{
			EISubSystem->AddMappingContext(IMC, 0);
		}
	}
}

void ABaseInGamePC::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	MyChara = Cast<ABaseInGameCharacter>(PawnToPossess);
	if (!MyChara)
	{
		return;
	}

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(MyChara->InputComponent);

	if (EIC)
	{
		if (IA_Move)
		{
			EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseInGamePC::Follow);
			EIC->BindAction(IA_Move, ETriggerEvent::Started, this, &AController::StopMovement);
			EIC->BindAction(IA_Move, ETriggerEvent::Completed | ETriggerEvent::Canceled, this, &ABaseInGamePC::SpawnDestinationSystem);
		}
	}
}

void ABaseInGamePC::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!MyChara)
	{
		return;
	}

	if (bIsMoving)
	{
		MyChara->AddMovementInput(MyChara->GetCharacterMovement()->Velocity.GetSafeNormal2D());
		double Dist = FVector::Dist(MyChara->GetActorLocation(), Destination + FVector(0, 0, 89.f));
		bIsMoving = (Dist > 50);
		bIsLerpMoving = (Dist <= 50);
	}
	if (bIsLerpMoving)
	{
		MyChara->AddMovementInput((Destination - MyChara->GetActorLocation()).GetSafeNormal2D());
		if (FVector::Dist(MyChara->GetActorLocation(), Destination + FVector(0, 0, 89.f)) < 20)
		{
			StopMovement();
			bIsLerpMoving = false;
		}
	}
}

void ABaseInGamePC::Follow()
{
	bIsMoving = CheckLand(Destination);
	if (!bIsMoving)
	{
		return;
	}

	double Dist = FVector::Dist(MyChara->GetActorLocation(), Destination);
	if (Dist < 20.f)
	{
		return;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
}

bool ABaseInGamePC::CheckLand(FVector& ImpactPoint)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TypeArray;
	TypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	FHitResult HitResult;
	bool IsHit = GetHitResultUnderCursorForObjects(TypeArray, true, HitResult);
	ImpactPoint = HitResult.ImpactPoint;

	return IsHit;
}

void ABaseInGamePC::SpawnDestinationSystem()
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleBinded"));
}
