#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "ChallengePawn.generated.h"

class UCapsuleComponent;
class SkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;


UENUM()
enum class EPawnState : uint8
{
	Grounded,
	Flying
};

UCLASS()
class ASSIGNMENT7_API AChallengePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChallengePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkelMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComp;
	
	
	//Action Functions
	void PawnMove(const FInputActionValue& value);
	
	void PawnLook(const FInputActionValue& value);
	
	// Movement Values
	float TargetPitch = 0.0f;
	float MoveSpeed = 0.0f;
	
	float CurrentRollAngle = 0.0f;
	float RollDirection = 1.0f;
	float RollSpeed = 360.0f;
	
	EPawnState PawnState = EPawnState::Flying;
	
	// gravity
	float VerticalVelocity = 0.0f;
	float Gravity = -980.0f;

};
