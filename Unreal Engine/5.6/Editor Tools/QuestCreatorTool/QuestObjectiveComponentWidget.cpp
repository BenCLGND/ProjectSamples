// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTools/QuestObjectiveComponentWidget.h"
#include "CustomTools/QuestTaskComponentWidget.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "Structs/EnemyStructs.h"
#include "Structs/InventoryStructs.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "SuqsQuest.h"

void UQuestObjectiveComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AddTaskButton->OnClicked.AddDynamic(this, &UQuestObjectiveComponentWidget::AddTask);
	RemoveTaskButton->OnClicked.AddDynamic(this, &UQuestObjectiveComponentWidget::RemoveTask);
	SuqsTasks.Empty();
}

void UQuestObjectiveComponentWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UQuestObjectiveComponentWidget::AddTask()
{
	UQuestTaskComponentWidget* TaskToAdd = CreateWidget<UQuestTaskComponentWidget>(this, TaskWidgetReference);
	TaskToAdd->SetTaskNumber(FString::FromInt(TaskVerticalBox->GetChildrenCount() + 1));
	TaskVerticalBox->AddChild(TaskToAdd);
}

void UQuestObjectiveComponentWidget::RemoveTask()
{
	if (TaskVerticalBox->GetChildrenCount() != 0)
	{
		TaskVerticalBox->RemoveChildAt(TaskVerticalBox->GetChildrenCount() - 1);
	}
	
}

FName UQuestObjectiveComponentWidget::GetIdentifier()
{
	return FName(*ObjectiveIdentifierInput->GetText().ToString());
}

FText UQuestObjectiveComponentWidget::GetTitle()
{
	return ObjectiveTitleInput->GetText();

}

FText UQuestObjectiveComponentWidget::GetActiveDescription()
{
	return ActiveDescriptionInput->GetText();
}

FText UQuestObjectiveComponentWidget::GetCompletedDescription()
{
	return CompletedDescriptionInput->GetText();
}

int UQuestObjectiveComponentWidget::GetNumberOfTasks()
{
	return FCString::Atoi(*TaskNumberInput->GetText().ToString());
}

bool UQuestObjectiveComponentWidget::GetIsSequential()
{
	return SequentialTaskCheckBox->IsChecked();
}

TArray<class UQuestTaskComponentWidget*> UQuestObjectiveComponentWidget::GetTasks()
{
	TArray<UQuestTaskComponentWidget*> ArrayToReturn;
	for ( int i = 0; i < TaskVerticalBox->GetChildrenCount(); i++)
	{
		ArrayToReturn.Add(Cast<UQuestTaskComponentWidget>(TaskVerticalBox->GetChildAt(i)));
		SuqsTasks.Add(Cast<UQuestTaskComponentWidget>(TaskVerticalBox->GetChildAt(i))->CreateSuqsTask());
	}
	return ArrayToReturn;
}

bool UQuestObjectiveComponentWidget::IsObjectiveValid()
{
	if (GetIdentifier() == "")
	{
		UE_LOG(LogTemp, Log, TEXT("Objective Identifier Invalid"));
		return false;
	}
	if (GetTitle().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective Title Invalid"));
		return false;
	}
	if (GetActiveDescription().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective Active Description Invalid"));
		return false;
	}
	if (GetCompletedDescription().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective Completed Description Invalid"));
		return false;
	}
	if (GetNumberOfTasks() == 0 || GetNumberOfTasks() < -1)
	{
		UE_LOG(LogTemp, Log, TEXT("Objective Task Number Invalid"));
		return false;
	}
	if (TaskVerticalBox->GetChildrenCount() <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("No Tasks on Objective Invalid"));
		return false;
	}
	for (int i = 0; i < TaskVerticalBox->GetChildrenCount(); i++)
	{
		if (!Cast<UQuestTaskComponentWidget>(TaskVerticalBox->GetChildAt(i))->IsTaskValid())
		{
			return false;
		}
	}


	return true;
}

void UQuestObjectiveComponentWidget::SetObjectiveText(FString Number)
{
	FString ObjectiveString = ObjectiveText->GetText().ToString() + Number;
	ObjectiveText->SetText(FText::FromString(ObjectiveString));
}

FSuqsObjective UQuestObjectiveComponentWidget::GetSuqsObjective()
{
	FSuqsObjective ObjectiveToReturn;
	ObjectiveToReturn.Identifier = GetIdentifier();
	ObjectiveToReturn.Title = GetTitle();
	ObjectiveToReturn.DescriptionWhenActive = GetActiveDescription();
	ObjectiveToReturn.DescriptionWhenCompleted = GetCompletedDescription();
	ObjectiveToReturn.bSequentialTasks = GetIsSequential();
	ObjectiveToReturn.NumberOfMandatoryTasksRequired = GetNumberOfTasks();
	ObjectiveToReturn.Tasks = SuqsTasks;
	return ObjectiveToReturn;
}
