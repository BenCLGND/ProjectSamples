// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SkillsMenuWidget.h"
#include "Widgets/ActionMenuWidget.h"
#include "Widgets/CustomButtonWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Combat/BattleManager.h"
#include "Components/VerticalBox.h"
#include "Skills/SkillData_PDA.h"
#include "System/UtilFunctionLibrary.h"
#include "Components/BattleStatsComponent.h"
#include "Player/DawnsEndPlayerController.h"

void USkillsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < ButtonVerticalBox->GetChildrenCount(); i++)
	{
		SkillButtonArray.Add(Cast<UCustomButtonWidget>(ButtonVerticalBox->GetChildAt(i)));
		ButtonVerticalBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	SkillButton_1->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_1);
	SkillButton_2->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_2);
	SkillButton_3->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_3);
	SkillButton_4->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_4);
	SkillButton_5->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_5);
	SkillButton_6->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::SkillButtonClicked_6);
}

void USkillsMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void USkillsMenuWidget::SkillButtonClicked_1(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(0);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::SkillButtonClicked_2(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(1);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::SkillButtonClicked_3(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(2);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::SkillButtonClicked_4(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(3);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::SkillButtonClicked_5(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(4);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::SkillButtonClicked_6(UCustomButtonWidget* Button)
{
	BattleManager->StartTargetPickMode(5);
	BattleManager->GetDEController()->CreatedBattleWidget->ActionMenu->HideSubMenu();
}

void USkillsMenuWidget::UpdateButtons(TArray<class USkillData_PDA*> Skills)
{
	for (int i = 0; i < SkillButtonArray.Num(); i++)
	{
		if (Skills.IsValidIndex(i)) {
			SkillButtonArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillButtonArray[i]->UpdateText(Skills[i]->SkillName);
			if (Skills[i]->ManaCost <= BattleManager->ActiveCharacter->BattleStatsComponent->State.MP) {
				SkillButtonArray[i]->SetIsEnabled(true);
			}
			else {
				SkillButtonArray[i]->SetIsEnabled(false);
			}
		}
		else {
			SkillButtonArray[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		
	}
}

void USkillsMenuWidget::UpdateButtons(TSet<FSkillField> Skills)
{
	TArray<FSkillField> SkillArray = Skills.Array();
	for (int i = 0; i < SkillButtonArray.Num(); i++)
	{
		if (SkillArray.IsValidIndex(i)) {
			SkillButtonArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillButtonArray[i]->UpdateText(SkillArray[i].SkillName);
			if (SkillArray[i].ManaCost <= BattleManager->ActiveCharacter->BattleStatsComponent->State.MP) {
				SkillButtonArray[i]->SetIsEnabled(true);
			}
			else {
				SkillButtonArray[i]->SetIsEnabled(false);
			}
		}
		else {
			SkillButtonArray[i]->SetVisibility(ESlateVisibility::Collapsed);
		}

	}
}

