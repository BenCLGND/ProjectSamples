// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "System/DE_Enums.h"
#include "Engine/Console.h"
#include "ConsoleSettings.h"
#include "Structs/InventoryStructs.h"
#include "OverworldCheatManagerExtension.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_END_API UOverworldCheatManagerExtension : public UCheatManagerExtension
{
	GENERATED_BODY()

public:
	static void PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList);

	virtual void AddedToCheatManager_Implementation() override;

protected:

	//Begin Day Cycle Subsystem Functions
	UFUNCTION(Exec)
	void SetTime(ETimeOfDay TimeToSet);

	//Begin Inventory Subsystem Functions
	UFUNCTION(Exec, meta = (Cheat = "Give Item"))
	void GiveItem(FName ItemToGive, int Quantity);

	UFUNCTION(Exec, meta = (Cheat = "Give Weapon"))
	void GiveWeapon(FName ItemToGive, int Quantity);

	UFUNCTION(Exec, meta = (Cheat = "Give Accessory"))
	void GiveAccessory(FName ItemToGive, int Quantity);

	UFUNCTION(Exec, meta = (Cheat = "Give Armor"))
	void GiveArmor(FName ItemToGive, int Quantity);

	UFUNCTION(Exec, meta = (Cheat = "Give KeyItem"))
	void GiveKeyItem(FName ItemToGive, int Quantity);

	UFUNCTION(Exec, meta = (Cheat = "Give Aurum"))
	void GiveAurum(int AurumToGive);

	//Begin SaveGame Subsystem Functions
	UFUNCTION(Exec, meta = (Cheat = "Save Game"))
	void SaveGame(FString SlotName);

	UFUNCTION(Exec, meta = (Cheat = "Load Game"))
	void LoadGame(FString SlotName);

	//Begin SUQS Functions
	UFUNCTION(Exec)
	void CompleteQuest(FName QuestID);

	UFUNCTION(Exec)
	void ProgressQuest(FName QuestID, FName TaskIdentifier);

	//Begin Level Functions
	UFUNCTION(Exec, meta = (Cheat = "Load Level"))
	void OpenLevel(FName LevelName);

	UFUNCTION(Exec)
	void RestartLevel();

	UFUNCTION(Exec, meta = (Cheat = "Teleport Level"))
	void TeleportToWaypoint(int WayPoint);

	UFUNCTION(Exec)
	void CancelEncounters();

	UFUNCTION(Exec)
	void EnableEncounters();

	UFUNCTION(Exec)
	void SpeedUp(bool SpeedUp);

	//Begin PartyMemberSubsystem Functions
	UFUNCTION(Exec)
	void IncreaseCharacterLevel();

	UFUNCTION(Exec)
	void AddExperience();

	UFUNCTION(Exec)
	void GiveJP(int JPToGive);


private:
	
	class UDawnsEndGameInstance* DEGameInst;
	class ADEGameModeBase* DEGameMode;
};
