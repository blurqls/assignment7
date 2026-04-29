#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChallengeController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASSIGNMENT7_API AChallengeController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	AChallengeController();
	
	void BeginPlay() override;
	
	//Input Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* LookAction;
};
