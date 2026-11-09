// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/PartyInfoWidget.h"
#include "W_Battle.generated.h"



class UHorizontalBox;
class UPartyInfoWidget;
class UOverlay;
/**
 * 
 */
UCLASS(BlueprintType)
class DAWNS_END_API UW_Battle : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta= (BindWidget))
	class UHorizontalBox* PortraitBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PartyStatBox;

	UPROPERTY()
	class ABattleManager* BattleManager;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* PauseOverlay;

	UFUNCTION()
	void ReplaceTurnOrder(TArray<class ABattleChar_BASE*> TurnOrder);

	UFUNCTION()
	void SortTurnPortraits(TArray<class ABattleChar_BASE*> TurnOrder);


	UFUNCTION()
	UPartyInfoWidget* GetCharacterWidget(int index);

	UFUNCTION(BlueprintCallable)
	void SetActionMenuVisible(bool SetVisible);

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ActionMenuReference;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UActionMenuWidget* ActionMenu;

	UFUNCTION()
	void CollapsePartyStatBox();

	UFUNCTION()
	void ShowBattleResultScreen(TSet<FInventorySlot> ItemsToShow, int AurumToShow, TArray<FLevelDisplayData> XPToSet);

	UFUNCTION()
	void ShowNotification(FText TextToSet);

	UFUNCTION()
	void ShowNotificationWithDelay(FText TextToSet, float Duration);

	UFUNCTION()
	void ShowPauseOverlay();

	UFUNCTION()
	void HidePauseOverlay();

	//FORCEINLINE class UActionMenuWidget* GetActionMenu() const { return ActionMenu; }

protected:

	//BeginPlay for Widgets
	//virtual bool Initialize() override;

private:
	UPROPERTY()
	class ADawnsEndPlayerController* DEController;

	UPROPERTY(meta = (BindWidget))
	class UNotificationWidget* NotificationWidget;

	UPROPERTY(meta = (BindWidget))
	class UBattleResultScreenWidget* BattleResultWidget;

};
