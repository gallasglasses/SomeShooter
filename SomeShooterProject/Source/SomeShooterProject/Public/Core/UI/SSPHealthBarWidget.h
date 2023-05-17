#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPHealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

UCLASS()
class SOMESHOOTERPROJECT_API USSPHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void OnTakeDamage();

	void SetHealthPercent(float Percent);

	void SetHealthBarStyle(const FProgressBarStyle& BarStyle);

	void SetScaleDamage(float Damage);

protected:

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScaleDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	 	UWidgetAnimation* OnTakeDamageAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float PercentVisibilityThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor CriticalHealthColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor WellHealthColor = FLinearColor::Green;
};
