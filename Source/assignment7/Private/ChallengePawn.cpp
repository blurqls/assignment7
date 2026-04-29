#include "ChallengePawn.h"

#include "ChallengeController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"


AChallengePawn::AChallengePawn()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	//Initialize
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(CapsuleComp);
	if (CapsuleComp) {CapsuleComp->SetSimulatePhysics(false);}
	
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton Mesh Component"));
	SkelMeshComp->SetupAttachment(CapsuleComp);
	if (SkelMeshComp){SkelMeshComp->SetSimulatePhysics(true);}
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 150.0f;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	

}


void AChallengePawn::BeginPlay()
{
	Super::BeginPlay();
	
}


void AChallengePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	UE_LOG(LogTemp, Warning, TEXT("PawnState: %s"), 
	PawnState == EPawnState::Grounded ? TEXT("Grounded") : TEXT("Flying"));
    
	FVector Start = GetActorLocation();
	float HalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	FVector End = Start + FVector(0.0f, 0.0f, -(HalfHeight + 5.0f));
    
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		// 위로 올라가는 중(점프)이면 그라운드 판정 무시
		if (VerticalVelocity > 0.0f)
		{
			PawnState = EPawnState::Flying;
			VerticalVelocity += Gravity * DeltaTime;
			FVector NewLocation = GetActorLocation();
			NewLocation.Z += VerticalVelocity * DeltaTime;
			SetActorLocation(NewLocation, true);
		}
		else
		{
			// 떨어지는 중이거나 정지 → 착지 처리
			PawnState = EPawnState::Grounded;
			VerticalVelocity = 0.0f;
			SetActorLocation(FVector(
				GetActorLocation().X,
				GetActorLocation().Y,
				HitResult.ImpactPoint.Z + HalfHeight
			));
		}
	}
	else
	{
		PawnState = EPawnState::Flying;
		VerticalVelocity += Gravity * DeltaTime;
		FVector NewLocation = GetActorLocation();
		NewLocation.Z += VerticalVelocity * DeltaTime;
		SetActorLocation(NewLocation, true);
	}
}


void AChallengePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);  
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AChallengeController* PawnController = Cast<AChallengeController>(GetController()))
		{
			if (PawnController->MoveAction)
			{
				EnhancedInput->BindAction(
					PawnController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AChallengePawn::PawnMove);
			}
			
			if (PawnController->LookAction)
			{
				EnhancedInput->BindAction(
					PawnController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AChallengePawn::PawnLook);
			}
		}
	}
}


void AChallengePawn::PawnMove(const FInputActionValue& value)
{
	const FVector InputValue = value.Get<FVector>();
	if (!InputValue.IsNearlyZero())
	{
		switch (PawnState)
		{
		case EPawnState::Grounded:
			MoveSpeed = 500.0f;
			break;
		case EPawnState::Flying:
			MoveSpeed = 500.0f * 0.5f;
			break;
		}

		float DeltaTime = GetWorld()->GetDeltaSeconds();
		
		FVector HorizontalInput = FVector(InputValue.X, InputValue.Y, 0.0f);
		AddActorLocalOffset(HorizontalInput * MoveSpeed * DeltaTime, true);
		
		if (!FMath::IsNearlyZero(InputValue.Z))
		{
			VerticalVelocity += InputValue.Z * 1200.0f * DeltaTime; // 추력
		}
	}
}

void AChallengePawn::PawnLook(const FInputActionValue& value)
{
	const FVector LookVector = value.Get<FVector>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	float RotationSpeed = 250.0f;
	

	AddActorLocalRotation(FRotator(0.0f, LookVector.X * RotationSpeed * DeltaTime, 0.0f));
	
	float DeltaPitch = LookVector.Y * RotationSpeed * DeltaTime * -1.0f;
	TargetPitch = FMath::Clamp(TargetPitch + DeltaPitch, -80.0f, 80.0f);
	SpringArmComp->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));
	
	float DeltaRoll = LookVector.Z * RotationSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(0.0f,0.0f,DeltaRoll));
	
}
