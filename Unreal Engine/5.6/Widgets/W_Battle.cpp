// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/W_Battle.h"
#include "Widgets/PortraitWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include <Kismet/GameplayStatics.h>
#include "Player/DawnsEndPlayerController.h"
#include "Widgets/NotificationWidget.h"
#include "System/UtilFunctionLibrary.h"
#include "Combat/BattleManager.h"
#include "Widgets/ActionMenuWidget.h"
#include "Widgets/BattleResultScreenWidget.h"
#include "Skills/SkillData_PDA.h"
#include "CustomLogging/CustomLogCategories.h"


void UW_Battle::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int i = 0; i < PartyStatBox->GetChildrenCount(); i++) 
	{
		PartyStatBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}
	for (int i = 0; i < PortraitBox->GetChildrenCount(); i++) {
		PortraitBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}
	DEController = Cast<ADawnsEndPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SetActionMenuVisible(false);
	//NotificationWidget = Cast<UNotificationWidget>(NotificationOverlay->GetChildAt(0));


	// Bind delegates here.

	

}

void UW_Battle::NativePreConstruct()
{

}

void UW_Battle::ReplaceTurnOrder(TArray<ABattleChar_BASE*> TurnOrder)
{
	//TurnOrder Breaks on Charge Turn
	if (TurnOrder.IsEmpty()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Array Empty")));
	}
	else {
		for (int i = 0; i < TurnOrder.Num(); i++)
		{
			if (PortraitBox) {
				Cast<UPortraitWidget>(PortraitBox->GetChildAt(i))->UpdatePortraitImage(TurnOrder[i]->Portrait);
				TurnOrder[i]->PortraitWidget = Cast<UPortraitWidget>(PortraitBox->GetChildAt(i));
				TurnOrder[i]->PortraitWidget->UpdateName(FText::FromString(TurnOrder[i]->DisplayName.ToString()));
				TurnOrder[i]->PortraitWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Portrait Box Failed")));
				break;
			}
		}
	}
}

void UW_Battle::SortTurnPortraits(TArray<ABattleChar_BASE*> TurnOrder)
{
	for (int i = 0; i < TurnOrder.Num(); i++)
	{
		UE_LOGFMT(LogCombatInfo, Display, "Next in Order is {first}.", TurnOrder[0]->DisplayName);
		Cast<UPortraitWidget>(PortraitBox->GetChildAt(i))->UpdatePortraitImage(TurnOrder[i]->Portrait);
		TurnOrder[i]->PortraitWidget = Cast<UPortraitWidget>(PortraitBox->GetChildAt(i));
		TurnOrder[i]->PortraitWidget->UpdateName(FText::FromString(TurnOrder[i]->DisplayName.ToString()));
		TurnOrder[i]->PortraitWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}
}


UPartyInfoWidget* UW_Battle::GetCharacterWidget(int index)
{
	UPartyInfoWidget* WidgetToReturn = nullptr;
	if (PartyStatBox) {
		WidgetToReturn = Cast<UPartyInfoWidget>(PartyStatBox->GetChildAt(index));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Party Stat Box Failed")));
	}
	return WidgetToReturn;
}

void UW_Battle::SetActionMenuVisible(bool SetVisible) 
{
	if (SetVisible) {
		ActionMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ActionMenu->BattleManager = BattleManager;
		ActionMenu->SetChildrenBattleManagers();
		//ActionMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (DEController) {
			DEController->ActivateUIControls(this);
			ActionMenu->SetChildrenActive();

		}
		else {
			printFStringColor("No Controller Found", Red));
			
		}
	}
	else {
		ActionMenu->SetVisibility(ESlateVisibility::Collapsed);
		if (DEController) {
			DEController->DeactivateUIControls();
		}
	}
}

void UW_Battle::CollapsePartyStatBox()
{
	PartyStatBox->SetVisibility(ESlateVisibility::Collapsed);
}


void UW_Battle::ShowBattleResultScreen(TSet<FInventorySlot> ItemsToShow, int AurumToShow, TArray<FLevelDisplayData> XPToSet)
{
	BattleResultWidget->AddDrops(ItemsToShow);
	BattleResultWidget->AddAurum(AurumToShow);
	BattleResultWidget->ShowPartyXP(XPToSet);
	BattleResultWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UW_Battle::ShowNotification(FText TextToSet)
{
	if (NotificationWidget) {
		NotificationWidget->SetMessageText(TextToSet);
	}
	NotificationWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UW_Battle::ShowNotificationWithDelay(FText TextToSet, float Duration)
{
	if (NotificationWidget) {
		NotificationWidget->SetMessageText(TextToSet);
	}
	UE_LOGFMT(LogCombatInfo, Display, "Showing Notification");
	NotificationWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (BattleManager->GetCanEndBattle())
	{
		NotificationWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	DELAY(0.5f, { NotificationWidget->SetVisibility(ESlateVisibility::Hidden);
	UE_LOGFMT(LogCombatInfo, Display, "Hiding Notification"); });
}

void UW_Battle::ShowPauseOverlay()
{
	PauseOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UW_Battle::HidePauseOverlay()
{
	PauseOverlay->SetVisibility(ESlateVisibility::Collapsed);
}
