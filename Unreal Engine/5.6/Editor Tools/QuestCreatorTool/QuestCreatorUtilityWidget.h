// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Structs/InventoryStructs.h"
#include "Structs/QuestStructs.h"
#include "QuestCreatorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_ENDEDITOR_API UQuestCreatorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;


	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* DataTableKey;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* QuestIdentifierInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* QuestTitleInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddLabelButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LabelVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveLabelButton;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ActiveDescriptionInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* CompletedDescriptionInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* AutoAcceptCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* ResolveAutomaticallyBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddObjectiveButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ObjectiveVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveObjectiveButton;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ExperienceRewardInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* AurumRewardInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddItemButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ItemToAddBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveItemButton;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddQuestButton;

	UPROPERTY(EditAnywhere, Category = "Objective Widget Reference")
	TSubclassOf<class UQuestObjectiveComponentWidget> ObjectiveWidgetReference;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.ItemField"))
	UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.KeyItemField"))
	UDataTable* KeyItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.WeaponField"))
	UDataTable* WeaponTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.ArmorField"))
	UDataTable* ArmorTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.AccessoryField"))
	UDataTable* AccessoryTable;

	UPROPERTY(EditAnywhere, Category = "Item Box Widget Reference")
	TSubclassOf<class UContainerUtilityItemBoxWidget> ItemBoxWidgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References")
	TMap<FString, class UDataTable*> QuestDataTableMap;


private:

	class UDEUnrealEditorSubsystem* EditorSubsystem;
	class UAssetEditorSubsystem* AssetSubsystem;
	TArray<class UQuestTaskComponentWidget*> Tasks;



	void CreateDataTableKey();

	UFUNCTION()
	void AddQuestToTable();

	UFUNCTION()
	void AddItem();

	UFUNCTION()
	void RemoveItem();

	UFUNCTION()
	void AddObjective();

	UFUNCTION()
	void RemoveObjective();

	TSet<FInventorySlot> GetItem();
	TMap<FName, FTaskRequirements> CreateTaskRequirements();

	bool IsQuestValid();
	bool AreItemsValid();
};
