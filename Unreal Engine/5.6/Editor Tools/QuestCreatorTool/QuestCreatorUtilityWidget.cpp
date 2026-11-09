// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTools/QuestCreatorUtilityWidget.h"
#include "CustomTools/DEUnrealEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Dawns_End/Public/CustomLogging/CustomLogCategories.h"
#include <Kismet/KismetMathLibrary.h>
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "CustomTools/ContainerUtilityItemBoxWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "CustomTools/QuestObjectiveComponentWidget.h"
#include "CustomTools/QuestTaskComponentWidget.h"
#include "Subsystems/AssetEditorSubsystem.h"

void UQuestCreatorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EditorSubsystem = Cast<UDEUnrealEditorSubsystem>(GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>());
	AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	AddQuestButton->OnClicked.AddDynamic(this, &UQuestCreatorUtilityWidget::AddQuestToTable);
	AddItemButton->OnClicked.AddDynamic(this, &UQuestCreatorUtilityWidget::AddItem);
	RemoveItemButton->OnClicked.AddDynamic(this, &UQuestCreatorUtilityWidget::RemoveItem);
	AddObjectiveButton->OnClicked.AddDynamic(this, &UQuestCreatorUtilityWidget::AddObjective);
	RemoveObjectiveButton->OnClicked.AddDynamic(this, &UQuestCreatorUtilityWidget::RemoveObjective);
	CreateDataTableKey();
	Tasks.Empty();

}

void UQuestCreatorUtilityWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}


void UQuestCreatorUtilityWidget::CreateDataTableKey()
{
	DataTableKey->AddOption("None");

	for (auto iter : QuestDataTableMap)
	{
		DataTableKey->AddOption(iter.Key);
		AssetSubsystem->CloseAllEditorsForAsset(iter.Value);
	}

	DataTableKey->SetSelectedOption("None");

}

void UQuestCreatorUtilityWidget::AddQuestToTable()
{
	if (!IsQuestValid())
	{
		return;
	}
	AssetSubsystem->CloseAllEditorsForAsset(QuestDataTableMap[DataTableKey->GetSelectedOption()]);
	FDawnsEndQuest DataToInsert;
	//Rewards
	DataToInsert.AurumReward = FCString::Atoi(*AurumRewardInput->GetText().ToString());
	DataToInsert.ExperienceReward = FCString::Atoi(*ExperienceRewardInput->GetText().ToString());
	if (ItemToAddBox->GetChildrenCount() > 0)
	{
		DataToInsert.ItemRewards = GetItem();
	}
	for (int i = 0; i < ObjectiveVerticalBox->GetChildrenCount(); i++)
	{
		Tasks.Append(Cast<UQuestObjectiveComponentWidget>(ObjectiveVerticalBox->GetChildAt(i))->GetTasks());
		DataToInsert.Objectives.Add(Cast<UQuestObjectiveComponentWidget>(ObjectiveVerticalBox->GetChildAt(i))->GetSuqsObjective());
	}
	//Quest
	DataToInsert.TaskRequirements = CreateTaskRequirements();
	DataToInsert.Identifier = FName(*QuestIdentifierInput->GetText().ToString());
	DataToInsert.Title = QuestTitleInput->GetText();
	DataToInsert.DescriptionWhenActive = ActiveDescriptionInput->GetText();
	DataToInsert.DescriptionWhenCompleted = CompletedDescriptionInput->GetText();

	//Objectives
	

	QuestDataTableMap[DataTableKey->GetSelectedOption()]->AddRow(FName(*QuestIdentifierInput->GetText().ToString()), DataToInsert);
	AssetSubsystem->OpenEditorForAsset(QuestDataTableMap[DataTableKey->GetSelectedOption()]);
}

void UQuestCreatorUtilityWidget::AddItem()
{
	UContainerUtilityItemBoxWidget* ItemToAdd = CreateWidget<UContainerUtilityItemBoxWidget>(this, ItemBoxWidgetReference);
	ItemToAddBox->AddChild(ItemToAdd);
}

void UQuestCreatorUtilityWidget::AddObjective()
{
	UQuestObjectiveComponentWidget* ObjectiveToAdd = CreateWidget<UQuestObjectiveComponentWidget>(this, ObjectiveWidgetReference);
	ObjectiveToAdd->SetObjectiveText(FString::FromInt(ObjectiveVerticalBox->GetChildrenCount() + 1));
	ObjectiveVerticalBox->AddChild(ObjectiveToAdd);
}

void UQuestCreatorUtilityWidget::RemoveItem()
{
	if (ItemToAddBox->GetChildrenCount() != 0)
	{
		ItemToAddBox->RemoveChildAt(ItemToAddBox->GetChildrenCount() - 1);
	}
}

void UQuestCreatorUtilityWidget::RemoveObjective()
{
	if (ObjectiveVerticalBox->GetChildrenCount() != 0)
	{
		ObjectiveVerticalBox->RemoveChildAt(ObjectiveVerticalBox->GetChildrenCount() - 1);
	}
}

TSet<FInventorySlot> UQuestCreatorUtilityWidget::GetItem()
{
	TSet<FInventorySlot> ItemsToReturn;
	if (ItemToAddBox->GetChildrenCount() == 0)
	{
		return ItemsToReturn;
	}
	for (int i = 0; i < ItemToAddBox->GetChildrenCount(); i++)
	{
		UContainerUtilityItemBoxWidget* ChildToCheck = Cast<UContainerUtilityItemBoxWidget>(ItemToAddBox->GetChildAt(i));
		if (!ChildToCheck)
		{
			UE_LOGFMT(LogTemp, Error, "ItemWidget Not Valid!");
			continue;
		}
		if (ChildToCheck->GetItemFromKeys().TableRow.IsNull())
		{
			UE_LOG(LogTemp, Log, TEXT("Item Table Invalid"));
		}
		if (ItemsToReturn.Contains(ChildToCheck->GetItemFromKeys()))
		{
			ItemsToReturn.Find(ChildToCheck->GetItemFromKeys())->Quantity += ChildToCheck->GetItemFromKeys().Quantity;
		}
		else {
			ItemsToReturn.Add(ChildToCheck->GetItemFromKeys());
		}
	}

	return ItemsToReturn;
}

TMap<FName, FTaskRequirements> UQuestCreatorUtilityWidget::CreateTaskRequirements()
{
	TMap<FName, FTaskRequirements> Requirements;
	for (int i = 0; i < Tasks.Num(); i++)
	{
		if (Tasks[i]->NeedsRequirements() == 2)
		{
			continue;
		}
		FTaskRequirements TaskRequirementsToAdd;
		if (Tasks[i]->NeedsRequirements() == 0)
		{
			TaskRequirementsToAdd.RequiredEnemy = Tasks[i]->GetEnemyFromKeys();
		}
		else if (Tasks[i]->NeedsRequirements() == 1)
		{
			TaskRequirementsToAdd.RequiredItem = Tasks[i]->GetItemFromKeys();
		}
		Requirements.Add(Tasks[i]->GetIdentifier(), TaskRequirementsToAdd);
	}
	return Requirements;
}

bool UQuestCreatorUtilityWidget::IsQuestValid()
{
	if (DataTableKey->GetSelectedOption() == "None")
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Table Invalid"));
		return false;
	}
	if (QuestIdentifierInput->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Identifier Invalid"));
		return false;
	}
	if (QuestTitleInput->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Title Invalid"));
		return false;
	}
	if (ActiveDescriptionInput->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Active Description Invalid"));
		return false;
	}
	if (CompletedDescriptionInput->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Completed Description Invalid"));
		return false;
	}
	if (!AreItemsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Item Rewards Invalid"));
		return false;
	}
	if (ObjectiveVerticalBox->GetChildrenCount() <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Objective Count Invalid"));
		return false;
	}
	for (int i = 0; i < ObjectiveVerticalBox->GetChildrenCount(); i++)
	{
		if (!Cast<UQuestObjectiveComponentWidget>(ObjectiveVerticalBox->GetChildAt(i))->IsObjectiveValid())
		{
			return false;
		}
	}
	

	return true;
}

bool UQuestCreatorUtilityWidget::AreItemsValid()
{
	for (int i = 0; i < ItemToAddBox->GetChildrenCount(); i++)
	{
		UContainerUtilityItemBoxWidget* ChildToCheck = Cast<UContainerUtilityItemBoxWidget>(ItemToAddBox->GetChildAt(i));
		if (!ChildToCheck)
		{
			UE_LOGFMT(LogTemp, Error, "ItemWidget Not Valid!");
			continue;
		}
		if (ChildToCheck->GetItemFromKeys().Quantity <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Item Quantity Invalid"));
			return false;
		}
		if (ChildToCheck->GetItemFromKeys().TableRow.IsNull())
		{
			UE_LOG(LogTemp, Log, TEXT("Item Table Invalid"));
			return false;
		}
	}
	return true;
}
