#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPCoreTypes.h"
#include "SSPLevelSelectorItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class SOMESHOOTERPROJECT_API USSPLevelSelectorItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FOnLevelSelectedSignature OnLevelSelected;

	void SetLevelData(const FLevelData& Data);
	FLevelData GetLevelData() const { return LevelData;}

	void SetSelected(bool IsSelected);

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* SelectLevelButton;

	UPROPERTY(meta = (BindWidget))
		UImage* SelectLevelImage = nullptr;

	UPROPERTY(meta = (BindWidget))
		UImage* FrameImage;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SelectLevelNameText;

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
		void OnLevelItemClicked();

	FLevelData LevelData;
	
};
