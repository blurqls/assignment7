#include "NewController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

ANewController::ANewController() 
	: InputMappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr) {}

void ANewController::BeginPlay()
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
