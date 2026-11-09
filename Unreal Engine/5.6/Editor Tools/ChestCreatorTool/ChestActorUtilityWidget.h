// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "System/DE_Enums.h"
#include "Structs/InventoryStructs.h"
#include "ChestActorUtilityWidget.generated.h"


/**
 * 
 */
UCLASS()
class DAWNS_ENDEDITOR_API UChestActorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:


	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;




	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* ContainerKey;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityComboBoxString* ChestKey;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* FurnitureTypeBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ChestTypeBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* AddItemButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ItemToAddBox;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* RemoveItemButton;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityEditableText* AurumAmountText;

	UPROPERTY(meta = (BindWidget))
	class UEditorUtilityButton* CreateChestButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, class UStaticMesh*> TrunkMeshMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, class UStaticMesh*> LidMeshMap;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TMap<FString, class UAnimationAsset*> AnimationMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATreasureChestActor> TreasureChestBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFurnitureActor> FurnitureBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UContainerUtilityItemBoxWidget> ItemBoxWidgetReference;


private:

	class UDEUnrealEditorSubsystem* EditorSubsystem;
	class UEditorActorSubsystem* ActorSubsystem;
	
	UFUNCTION(BlueprintCallable)
	void CheckContainerKey(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable)
	void CreateContainerActor();

	UFUNCTION(BlueprintCallable)
	void AddItemWidget();

	UFUNCTION(BlueprintCallable)
	void RemoveItemWidget();

	void CreateChestKey();
	void CreateContainerKey();
	TSet<FInventorySlot> GetItem();

	void CreateChestActor(class UStaticMesh* TrunkMeshToSet, class UStaticMesh* LidMeshToSet, TSet<FInventorySlot> ItemsToAdd, int Aurum);
	void CreateFurnitureActor(UStaticMesh* MeshToSet, FInventorySlot ItemToAdd, int Aurum);

	bool PerformLineTraceForSpawn(FVector CameraLocation, FVector& TraceEnd, FHitResult& Hit);

};
