// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Structs/InventoryStructs.h"
#include "Structs/QuestStructs.h"
#include "QuestTaskComponentWidget.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_ENDEDITOR_API UQuestTaskComponentWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TaskText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* TaskIdentifierInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* TaskTitleInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddLabelButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LabelVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveLabelButton;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* TaskTypeKey;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnemyText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* EnemyKey;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* ItemKey;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TargetNumberText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* TargetNumberInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* MandatoryTaskCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* ResolveDelayInput;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityCheckBox* ResolveAutomaticallyBox;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.EnemyFields"))
	UDataTable* EnemyTable;

	UPROPERTY()
	TMap<FString, UDataTable*> ItemTableMap;

	UFUNCTION()
	FInventorySlot GetItemFromKeys();

	UFUNCTION()
	FEnemyRequirements GetEnemyFromKeys();

	UFUNCTION()
	FName GetIdentifier();

	UFUNCTION()
	FText GetTitle();

	UFUNCTION()
	bool GetIsMandatory();

	UFUNCTION()
	bool IsTaskValid();

	UFUNCTION()
	int NeedsRequirements();

	UFUNCTION()
	FSuqsTask CreateSuqsTask();

	UFUNCTION()
	void SetTaskNumber(FString Number);

private:

	TMap<FString, FName> EnemyRowNameMap;

	UFUNCTION()
	FString GetKeyOption(FString Type);

	UFUNCTION()
	int GetAmount();

	void CreateItemKey();

	void CreateEnemyKey();

	void CreateTaskKey();

	UFUNCTION()
	void CheckTaskTypeKey(FString SelectedItem, ESelectInfo::Type SelectionType);

};
