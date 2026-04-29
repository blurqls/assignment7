#include "NewPawn.h"
#include "NewController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ANewPawn::ANewPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
    SetRootComponent(CapsuleComp);
    if (CapsuleComp) {CapsuleComp->SetSimulatePhysics(false);}

    SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
    SkelMeshComp->SetupAttachment(CapsuleComp);
    if (SkelMeshComp) {SkelMeshComp->SetSimulatePhysics(false);}

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
    SpringArmComp->SetupAttachment(CapsuleComp);
    SpringArmComp->TargetArmLength = 100.0f;
    SpringArmComp->bUsePawnControlRotation = false;
	
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComp->SetupAttachment(SpringArmComp);
    CameraComp->bUsePawnControlRotation = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(CapsuleComp);
}

void ANewPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ANewPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ANewController* PawnController = Cast<ANewController>(GetController()))
        {
            if (PawnController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PawnController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANewPawn::MoveAction);
            }

            if (PawnController->LookAction)
            {
                EnhancedInput->BindAction(
                    PawnController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANewPawn::LookAction);
            }
        }
    }
}

void ANewPawn::MoveAction(const FInputActionValue& value)
{
	const FVector2D InputValue = value.Get<FVector2D>();
	
	if (InputValue.SizeSquared() > 0)
	{
		MoveSpeed = 500.0f;
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		
		FVector DeltaLocation = FVector(InputValue.X,InputValue.Y,0)*MoveSpeed*DeltaTime;
		
		AddActorLocalOffset(DeltaLocation,true);
	}
}

void ANewPawn::LookAction(const FInputActionValue& value)
{
	// 1. 마우스/스틱 입력값 가져오기 (X: 좌우, Y: 상하)
	FVector2D LookInput = value.Get<FVector2D>();
    
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float RotationSpeed = 50.0f;
	
	
	float DeltaYaw = LookInput.X * RotationSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(0.0f, DeltaYaw, 0.0f));

	float DeltaPitch = LookInput.Y * RotationSpeed * DeltaTime * -1.0f;
	
	TargetPitch = FMath::Clamp(TargetPitch + DeltaPitch, -80.0f, 80.0f);
	
	SpringArmComp->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));
}