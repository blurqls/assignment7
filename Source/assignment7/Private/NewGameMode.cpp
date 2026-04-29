#include "NewGameMode.h"
#include "NewController.h"
#include "NewPawn.h"

ANewGameMode::ANewGameMode()
{
	DefaultPawnClass = ANewPawn::StaticClass();
	PlayerControllerClass = ANewController::StaticClass();
}
