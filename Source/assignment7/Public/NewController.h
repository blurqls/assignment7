#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NewController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASSIGNMENT7_API ANewController : public APlayerController
{
	GENERATED_BODY()

public:
	ANewController();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	
	void BeginPlay() override;
};
