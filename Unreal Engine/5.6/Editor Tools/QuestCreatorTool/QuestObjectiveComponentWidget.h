// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "SuqsQuest.h"
#include "QuestObjectiveComponentWidget.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_ENDEDITOR_API UQuestObjectiveComponentWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ObjectiveText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ObjectiveIdentifierInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ObjectiveTitleInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ActiveDescriptionInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* CompletedDescriptionInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* SequentialTaskCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* TaskNumberInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* ContinueOnFailCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddTaskButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* TaskVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveTaskButton;

	UPROPERTY(EditAnywhere, Category = "Task Widget Reference")
	TSubclassOf<class UQuestTaskComponentWidget> TaskWidgetReference;

	UFUNCTION()
	FName GetIdentifier();

	UFUNCTION()
	FText GetTitle();

	UFUNCTION()
	FText GetActiveDescription();

	UFUNCTION()
	FText GetCompletedDescription();

	UFUNCTION()
	int GetNumberOfTasks();

	UFUNCTION()
	bool GetIsSequential();

	UFUNCTION()
	TArray<class UQuestTaskComponentWidget*> GetTasks();

	UFUNCTION()
	bool IsObjectiveValid();

	UFUNCTION()
	void SetObjectiveText(FString Number);

	UFUNCTION()
	FSuqsObjective GetSuqsObjective();

private:

	TArray<FSuqsTask> SuqsTasks;

	UFUNCTION()
	void AddTask();

	UFUNCTION()
	void RemoveTask();
};
