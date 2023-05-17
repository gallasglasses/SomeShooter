#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SOMESHOOTERPROJECT_API USSPPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "1", ClampMax = "2"))
		int32 TeamColorID = 1;

	virtual void NativePreConstruct() override;
	void SetPlayerName(const FText& Text);
	void SetKills(const FText& Text);
	void SetDeaths(const FText& Text);
	//void SetTeam(const FText& Text);
	void SetPlayerIndicatorVisibility(bool IsVisible);
	void SetPlayerIndicatorColor(int32 TeamID);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor Team1Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor Team2Color;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerNameText; 

	UPROPERTY(meta = (BindWidget))
		UTextBlock* KillsText; 

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DeathsText; 

	/*UPROPERTY(meta = (BindWidget))
		UTextBlock* TeamText;*/ 

	UPROPERTY(meta = (BindWidget))
		UImage* PlayerIndicatorImage;
};
