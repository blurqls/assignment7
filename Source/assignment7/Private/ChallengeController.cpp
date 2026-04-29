#include "ChallengeController.h"

#include "EnhancedInputSubsystems.h"

AChallengeController::AChallengeController() 
	:InputMappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr){}

void AChallengeController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPawn = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPawn->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext,0);
			}
		}
	}
}
