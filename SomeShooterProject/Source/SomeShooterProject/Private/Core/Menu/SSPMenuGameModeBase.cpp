#include "Core/Menu/SSPMenuGameModeBase.h"
#include "Core/Menu/SSPMenuPlayerController.h"
#include "Core/Menu/UI/SSPMenuHUD.h"

ASSPMenuGameModeBase::ASSPMenuGameModeBase()
{
    PlayerControllerClass = ASSPMenuPlayerController::StaticClass();
    HUDClass = ASSPMenuHUD::StaticClass();
}
