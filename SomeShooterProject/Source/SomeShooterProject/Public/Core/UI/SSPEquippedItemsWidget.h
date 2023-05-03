#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPCoreTypes.h"
#include "SSPEquippedItemsWidget.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPEquippedItemsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponType(ECharacterWeapon& WeaponType) const;
};
