// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTools/QuestTaskComponentWidget.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "Structs/EnemyStructs.h"
#include "Structs/InventoryStructs.h"
#include "Components/TextBlock.h"
#include "SuqsQuest.h"

void UQuestTaskComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TaskTypeKey->OnSelectionChanged.AddUniqueDynamic(this, &UQuestTaskComponentWidget::CheckTaskTypeKey);
	CreateItemKey();
	CreateEnemyKey();
	CreateTaskKey();
}

void UQuestTaskComponentWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}


FEnemyRequirements UQuestTaskComponentWidget::GetEnemyFromKeys()
{
	FDataTableRowHandle EnemyRowHandle;
	if (GetKeyOption("Enemy") == "None")
	{
		EnemyRowHandle.DataTable = nullptr;
		EnemyRowHandle.RowName = FName("None");
	}
	else
	{
		EnemyRowHandle.DataTable = EnemyTable;
		EnemyRowHandle.RowName = EnemyRowNameMap[GetKeyOption("Enemy")];
	}


	FEnemyRequirements Enemy;
	Enemy.TableRow = EnemyRowHandle;
	Enemy.NumberToDefeat = GetAmount();
	return Enemy;
}

FInventorySlot UQuestTaskComponentWidget::GetItemFromKeys()
{
	FDataTableRowHandle ItemRowHandle;
	if (GetKeyOption("Item") == "None")
	{
		ItemRowHandle.DataTable = nullptr;
		ItemRowHandle.RowName = FName("None");
	}
	else 
	{
		ItemRowHandle.DataTable = ItemTableMap[GetKeyOption("Item")];
		ItemRowHandle.RowName = FName(*GetKeyOption("Item"));
	}
	

	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = GetAmount();
	return Item;
}

FString UQuestTaskComponentWidget::GetKeyOption(FString Type)
{
	if (Type == "Enemy")
	{
		return EnemyKey->GetSelectedOption();
	}
	else if (Type == "Item")
	{
		return ItemKey->GetSelectedOption();
	}
	else {
		return "None";
	}
	
}

int UQuestTaskComponentWidget::GetAmount()
{
	return FCString::Atoi(*TargetNumberInput->GetText().ToString());
}

void UQuestTaskComponentWidget::CreateItemKey()
{
	ItemKey->AddOption("None");
	//Get Item Names 
	for (auto iter : ItemTable->GetRowMap())
	{
		FItemField* data = (FItemField*)(iter.Value);
		ItemKey->AddOption(data->RowNameReference.ToString());
		ItemTableMap.Add(data->RowNameReference.ToString(), ItemTable);
	}
	//Get Weapon Names
	for (auto iter : WeaponTable->GetRowMap())
	{
		FWeaponField* data = (FWeaponField*)(iter.Value);
		ItemKey->AddOption(data->RowNameReference.ToString());
		ItemTableMap.Add(data->RowNameReference.ToString(), WeaponTable);
	}
	//Get Armor Names
	for (auto iter : ArmorTable->GetRowMap())
	{
		FArmorField* data = (FArmorField*)(iter.Value);
		ItemKey->AddOption(data->RowNameReference.ToString());
		ItemTableMap.Add(data->RowNameReference.ToString(), ArmorTable);
	}
	//Get KeyItem Names
	for (auto iter : KeyItemTable->GetRowMap())
	{
		FKeyItemField* data = (FKeyItemField*)(iter.Value);
		ItemKey->AddOption(data->RowNameReference.ToString());
		ItemTableMap.Add(data->RowNameReference.ToString(), KeyItemTable);
	}
	//Get Accessory Names
	for (auto iter : AccessoryTable->GetRowMap())
	{
		FAccessoryField* data = (FAccessoryField*)(iter.Value);
		ItemKey->AddOption(data->RowNameReference.ToString());
		ItemTableMap.Add(data->RowNameReference.ToString(), AccessoryTable);
	}
	ItemKey->SetSelectedOption("None");
}

void UQuestTaskComponentWidget::CreateEnemyKey()
{
	EnemyKey->AddOption("None");
	for (auto iter : EnemyTable->GetRowMap())
	{
		FEnemyFields* data = (FEnemyFields*)(iter.Value);
		EnemyKey->AddOption(data->enemyName.ToString());
		EnemyRowNameMap.Add(data->enemyName.ToString(), iter.Key);
	}
	EnemyKey->SetSelectedOption("None");
}

void UQuestTaskComponentWidget::CreateTaskKey()
{
	TaskTypeKey->AddOption("None");
	TaskTypeKey->AddOption("Enemy");
	TaskTypeKey->AddOption("Item Fetch");
	TaskTypeKey->AddOption("Other");
	TaskTypeKey->SetSelectedOption("None");
}

void UQuestTaskComponentWidget::CheckTaskTypeKey(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Enemy")
	{
		
		ItemKey->SetVisibility(ESlateVisibility::Collapsed);
		ItemText->SetVisibility(ESlateVisibility::Collapsed);
		EnemyKey->SetVisibility(ESlateVisibility::Visible);
		EnemyText->SetVisibility(ESlateVisibility::Visible);
		TargetNumberInput->SetVisibility(ESlateVisibility::Visible);
		TargetNumberText->SetVisibility(ESlateVisibility::Visible);

	}
	else if (SelectedItem == "Item Fetch")
	{
		ItemKey->SetVisibility(ESlateVisibility::Visible);
		ItemText->SetVisibility(ESlateVisibility::Visible);
		EnemyKey->SetVisibility(ESlateVisibility::Collapsed);
		EnemyText->SetVisibility(ESlateVisibility::Collapsed);
		TargetNumberInput->SetVisibility(ESlateVisibility::Visible);
		TargetNumberText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemKey->SetVisibility(ESlateVisibility::Collapsed);
		ItemText->SetVisibility(ESlateVisibility::Collapsed);
		EnemyKey->SetVisibility(ESlateVisibility::Collapsed);
		EnemyText->SetVisibility(ESlateVisibility::Collapsed);
		TargetNumberInput->SetVisibility(ESlateVisibility::Collapsed);
		TargetNumberText->SetVisibility(ESlateVisibility::Collapsed);

	}
}

FName UQuestTaskComponentWidget::GetIdentifier()
{
	return FName(*TaskIdentifierInput->GetText().ToString());
}

FText UQuestTaskComponentWidget::GetTitle()
{
	return TaskTitleInput->GetText();

}

bool UQuestTaskComponentWidget::GetIsMandatory()
{
	return MandatoryTaskCheckBox->IsChecked();
}

bool UQuestTaskComponentWidget::IsTaskValid()
{
	if (GetIdentifier() == "")
	{
		UE_LOG(LogTemp, Log, TEXT("Task Identifier Invalid"));
		return false;
	}
	if (GetTitle().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Task Title Invalid"));
		return false;
	}
	if (TaskTypeKey->GetSelectedOption() == "None")
	{
		UE_LOG(LogTemp, Log, TEXT("Task Type Invalid"));
		return false;
	}
	if ((ItemKey->IsVisible() || EnemyKey->IsVisible()) && GetAmount() <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Task Requirement Amount Invalid"));
		return false;
	}	
	if ((ItemKey->IsVisible() && GetItemFromKeys().TableRow.IsNull()))
	{
		UE_LOG(LogTemp, Log, TEXT("Task Item Table is Invalid"));
		return false;
	}
	if ((EnemyKey->IsVisible() && GetEnemyFromKeys().TableRow.IsNull()))
	{
		UE_LOG(LogTemp, Log, TEXT("Task Enemy Row is Invalid"));
		return false;
	}
	return true;
}

//Return 0 for enemy, 1 for item, 2 for none
int UQuestTaskComponentWidget::NeedsRequirements()
{
	if (TaskTypeKey->GetSelectedOption() == "Enemy")
	{
		return 0;
	}
	else if (TaskTypeKey->GetSelectedOption() == "Item Fetch")
	{
		return 1;
	}
	else 
	{
		return 2;
	}
}

FSuqsTask UQuestTaskComponentWidget::CreateSuqsTask()
{
	FSuqsTask TaskToReturn;
	TaskToReturn.Identifier = GetIdentifier();
	TaskToReturn.Title = GetTitle();
	TaskToReturn.TargetNumber = 1;
	TaskToReturn.bMandatory = GetIsMandatory();
	return TaskToReturn;
}

void UQuestTaskComponentWidget::SetTaskNumber(FString Number)
{
	FString TaskString = TaskText->GetText().ToString() + Number;
	TaskText->SetText(FText::FromString(TaskString));
}
