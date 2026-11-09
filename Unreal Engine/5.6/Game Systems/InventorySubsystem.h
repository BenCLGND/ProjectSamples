// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/DE_Enums.h"
#include <Structs/InventoryStructs.h>
#include "InventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_END_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	// Override the virtual functions of USubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UFUNCTION()
	int UseItemFromInventory(struct FInventorySlot ItemToUse);

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(struct FInventorySlot ItemToAdd);

	UFUNCTION(BlueprintCallable)
	void AddAurum(int ValueToAdd);

	UFUNCTION(BlueprintCallable)
	bool SubtractAurum(int ValueToSubtract);

	UFUNCTION(BlueprintAuthorityOnly)
	FORCEINLINE TSet<FInventorySlot>& GetInventoryFromSubsystem() { return Inventory; }

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSet<FInventorySlot> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data Table References")
	TMap<EInventoryItemType, UDataTable*> InventoryTables;

	UPROPERTY(SaveGame)
	int AurumAmount = 0;


private:
	UPROPERTY()
	class UDawnsEndGameInstance* GameInst;

};
