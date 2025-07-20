#include "HW08PlayerController.h"
#include "EnhancedInputSubsystems.h"

AHW08PlayerController::AHW08PlayerController():
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{
}

void AHW08PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	
}
