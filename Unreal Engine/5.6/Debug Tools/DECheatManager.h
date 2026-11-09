// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "System/DE_Enums.h"
#include "Engine/Console.h"
#include "ConsoleSettings.h"
#include "Structs/InventoryStructs.h"
#include "DECheatManager.generated.h"


/**
 * 
 */
UCLASS()
class DAWNS_END_API UDECheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	static void PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList);
	void AddCustomCheatManagerExtension(class UOverworldCheatManagerExtension* CheatObject);
	void AddCustomCheatManagerExtension(class UCombatCheatManagerExtension* CheatObject);
	void RemoveCustomCheatManagerExtension(class UOverworldCheatManagerExtension* CheatObject);
	void RemoveCustomCheatManagerExtension(class UCombatCheatManagerExtension* CheatObject);

protected:


private:

};
