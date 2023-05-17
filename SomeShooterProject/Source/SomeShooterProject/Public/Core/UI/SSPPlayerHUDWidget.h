#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPCoreTypes.h"
#include "SSPPlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SOMESHOOTERPROJECT_API USSPPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	/*UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();*/

protected:

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* DamageAnimation;

	virtual void NativeOnInitialized() override;

private:

	void OnHealthChanged(float Health, float DeltaHealth);
	void OnNewPawn(APawn* NewPawn);
	void UpdateHealthBar();
};
