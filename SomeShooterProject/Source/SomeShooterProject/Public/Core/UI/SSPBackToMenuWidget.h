#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPBackToMenuWidget.generated.h"

class UButton;

UCLASS()
class SOMESHOOTERPROJECT_API USSPBackToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* BackToMenuButton;

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
		void OnBackToMenu();
	
};
