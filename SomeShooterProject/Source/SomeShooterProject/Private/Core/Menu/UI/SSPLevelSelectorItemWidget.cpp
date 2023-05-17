#include "Core/Menu/UI/SSPLevelSelectorItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USSPLevelSelectorItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;
	
	if (SelectLevelNameText)
	{
		SelectLevelNameText->SetText(FText::FromName(Data.LevelDisplayName));
	}

	if (SelectLevelImage)
	{
		SelectLevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void USSPLevelSelectorItemWidget::SetSelected(bool IsSelected)
{
	/*if (FrameImage)
	{
		FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}*/
	if (SelectLevelImage)
	{
		SelectLevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Blue : FLinearColor::White);
	}
}

void USSPLevelSelectorItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (SelectLevelButton)
	{
		SelectLevelButton->OnClicked.AddDynamic(this, &USSPLevelSelectorItemWidget::OnLevelItemClicked);
	}
}

void USSPLevelSelectorItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}