#include "Core/UI/SSPHealthBarWidget.h"
#include "SSPUtils.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AIController.h"

void USSPHealthBarWidget::SetHealthPercent(float Percent)
{
	if(!HealthProgressBar) return;

	const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))
										? ESlateVisibility::Hidden
										: ESlateVisibility::Visible;

	HealthProgressBar->SetVisibility(HealthBarVisibility);

	/*const auto HealthBarColor = Percent > PercentColorThreshold ? WellHealthColor : CriticalHealthColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);*/

	HealthProgressBar->SetPercent(Percent);
}

void USSPHealthBarWidget::SetHealthBarStyle(const FProgressBarStyle& BarStyle)
{
	if(!HealthProgressBar) return;

	HealthProgressBar->WidgetStyle = BarStyle;
}

void USSPHealthBarWidget::SetScaleDamage(float Damage)
{
	if (Damage < 0.0f)
	{
		ScaleDamage->SetVisibility(ESlateVisibility::Visible);
		ScaleDamage->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), -Damage)));
		OnTakeDamage();
		PlayAnimation(OnTakeDamageAnim);
	}
	else
	{
		ScaleDamage->SetVisibility(ESlateVisibility::Hidden);
	}
}
