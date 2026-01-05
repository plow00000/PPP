// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInGameCharacter.h"
#include "BaseInGamePC.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABaseInGameCharacter::ABaseInGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -89.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->SetRelativeRotation(FRotator(-65.f, 0, 0));
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABaseInGameCharacter::CheckCombo_Implementation()
{
	MyPC->CheckCombo_Implementation();
}

inline void ABaseInGameCharacter::StartCheckingCombo_Implementation()
{
	MyPC->StartCheckingCombo_Implementation();
}

void ABaseInGameCharacter::StopCheckingCombo_Implementation()
{
	MyPC->StopCheckingCombo_Implementation();
}

void ABaseInGameCharacter::MontageEnd_Implementation()
{
	MyPC->MontageEnd_Implementation();
}

// Called when the game starts or when spawned
void ABaseInGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseInGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

