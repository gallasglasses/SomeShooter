#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPPauseWidget.generated.h"

class UButton;

UCLASS()
class SOMESHOOTERPROJECT_API USSPPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected: 
	
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton; 
	
	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
		void OnContinue();
};
