#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSPGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOMESHOOTERPROJECT_API ASSPGameHUD : public AHUD
{
	GENERATED_BODY()
	
	public:

		virtual void DrawHUD() override;

	private:

		void DrawCrossHair();

};