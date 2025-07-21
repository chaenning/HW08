#include "HW08GameMode.h"
#include "HW08Character.h"
#include "HW08PlayerController.h"
#include "HW08GameState.h"

AHW08GameMode::AHW08GameMode()
{
	DefaultPawnClass = AHW08Character::StaticClass();
	PlayerControllerClass = AHW08PlayerController::StaticClass();
	GameStateClass = AHW08GameState::StaticClass();
}
