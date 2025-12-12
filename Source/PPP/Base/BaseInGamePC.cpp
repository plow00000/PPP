// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInGamePC.h"
#include "BaseInGameCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

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
		//bIsLerpMoving = (Dist <= 50);
	}
	else
	{
		StopMovement();
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

	/*double Dist = FVector::Dist(MyChara->GetActorLocation(), Destination);
	if (Dist < 20.f)
	{
		return;
	}*/
	
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

void ABaseInGamePC::SimpleMoveToLocation(const FVector& GoalLocation)
{

	UNavigationSystemV1* NavSys = this ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()) : nullptr;
	if (NavSys == nullptr || GetPawn() == nullptr)
	{
		UE_LOG(LogNavigation, Warning, TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(NavSys), *GetNameSafe(this), this ? *GetNameSafe(GetPawn()) : TEXT("NULL"));
		return;
	}

	UPathFollowingComponent* PFollowComp = FindComponentByClass<UPathFollowingComponent>();
	if (PFollowComp == nullptr)
	{
		PFollowComp = NewObject<UPathFollowingComponent>(this);
		PFollowComp->RegisterComponentWithWorld(GetWorld());
		PFollowComp->Initialize();
	}

	if (PFollowComp == nullptr)
	{
		return;
	}

	if (!PFollowComp->IsPathFollowingAllowed())
	{
		return;
	}
	bIsMoving = FVector::Dist(MyChara->GetActorLocation(), GoalLocation + FVector(0, 0, 89.f)) < 5.f;
	
	UE_LOG(LogTemp, Warning, TEXT("AtStart: %s"), *(StaticEnum<EPathFollowingStatus::Type>()->GetValueAsString(PFollowComp->GetStatus())));

	// script source, keep only one move request at time
	if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bIsMoving ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	}

	// script source, keep only one move request at time
	if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}

	UE_LOG(LogTemp, Warning, TEXT("AtBased: %s"), *(StaticEnum<EPathFollowingStatus::Type>()->GetValueAsString(PFollowComp->GetStatus())));


	if (bIsMoving)
	{
		PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	}
	else
	{
		const FVector AgentNavLocation = GetNavAgentLocation();
		const ANavigationData* NavData = NavSys->GetNavDataForProps(GetNavAgentPropertiesRef(), AgentNavLocation);
		if (NavData)
		{
			FPathFindingQuery Query(this, *NavData, AgentNavLocation, GoalLocation);
			FPathFindingResult Result = NavSys->FindPathSync(Query);
			UE_LOG(LogTemp, Warning, TEXT("AtMiddle:%s"), *(StaticEnum<EPathFollowingStatus::Type>()->GetValueAsString(PFollowComp->GetStatus())));
			if (Result.IsSuccessful())
			{
				PFollowComp->RequestMove(FAIMoveRequest(GoalLocation), Result.Path);
				UE_LOG(LogTemp, Warning, TEXT("Requested"));
				UE_LOG(LogTemp, Warning, TEXT("AtLast : %s"), *(StaticEnum<EPathFollowingStatus::Type>()->GetValueAsString(PFollowComp->GetStatus())));
			}
			else if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
			{
				PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
				UE_LOG(LogTemp, Warning, TEXT("Invalid"));
				UE_LOG(LogTemp, Warning, TEXT("AtLast : %s"), *(StaticEnum<EPathFollowingStatus::Type>()->GetValueAsString(PFollowComp->GetStatus())));
			}
		}
	}
}