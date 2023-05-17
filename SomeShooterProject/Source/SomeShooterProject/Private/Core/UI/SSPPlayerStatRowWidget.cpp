#include "Core/UI/SSPPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USSPPlayerStatRowWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerIndicatorImage->SetColorAndOpacity(TeamColorID == 1 ? Team1Color : Team2Color);
}
void USSPPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameText) return;
	PlayerNameText->SetText(Text);
}

void USSPPlayerStatRowWidget::SetKills(const FText& Text)
{
	if(!KillsText) return;
	KillsText->SetText(Text);
}

void USSPPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsText) return;
	DeathsText->SetText(Text);
}

//void USSPPlayerStatRowWidget::SetTeam(const FText& Text)
//{
//	if (!TeamText) return;
//	TeamText->SetText(Text);
//}

void USSPPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool IsVisible)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USSPPlayerStatRowWidget::SetPlayerIndicatorColor(int32 TeamID)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetColorAndOpacity(TeamID == 1 ? Team1Color : Team2Color);
}
