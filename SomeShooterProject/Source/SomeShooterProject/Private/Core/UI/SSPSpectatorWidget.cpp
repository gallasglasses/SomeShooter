#include "Core/UI/SSPSpectatorWidget.h"
#include "Core/Characters/Components/SSPRespawnComponent.h"
#include "SSPUtils.h"

bool USSPSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComponent = SSPUtils::GetPlayerComponent<USSPRespawnComponent>(GetOwningPlayer());
    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

    CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}
