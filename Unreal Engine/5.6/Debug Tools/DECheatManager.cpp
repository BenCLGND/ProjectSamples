// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTools/DECheatManager.h"
#include "Engine/Console.h"
#include "System/DE_Enums.h"
#include <System/DawnsEndGameInstance.h>
#include "System/InventorySubsystem.h"
#include "Structs/InventoryStructs.h"
#include "System/PartyMemberSubsystem.h"
#include "System/EquipmentSubsystem.h"
#include "System/ClassSubsystem.h"
#include "ConsoleSettings.h"
#include "DebugTools/OverworldCheatManagerExtension.h"
#include "DebugTools/CombatCheatManagerExtension.h"

void UDECheatManager::PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList)
{
#if UE_WITH_CHEAT_MANAGER


#endif
}

void UDECheatManager::AddCustomCheatManagerExtension(UOverworldCheatManagerExtension* CheatObject)
{
	if (ensure(CheatObject))
	{
		if (!CheatManagerExtensions.Contains(CheatObject))
		{
			CheatManagerExtensions.Add(CheatObject);
			CheatObject->AddedToCheatManager();
		}
	}
}

void UDECheatManager::AddCustomCheatManagerExtension(UCombatCheatManagerExtension* CheatObject)
{
	if (ensure(CheatObject))
	{
		if (!CheatManagerExtensions.Contains(CheatObject))
		{
			CheatManagerExtensions.Add(CheatObject);
			CheatObject->AddedToCheatManager();
		}
	}
}

void UDECheatManager::RemoveCustomCheatManagerExtension(UOverworldCheatManagerExtension* CheatObject)
{
	if (ensure(CheatObject))
	{
		if (CheatManagerExtensions.Contains(CheatObject))
		{
			CheatManagerExtensions.Remove(CheatObject);
			CheatObject->RemovedFromCheatManager();
		}
	}
}

void UDECheatManager::RemoveCustomCheatManagerExtension(UCombatCheatManagerExtension* CheatObject)
{
	if (ensure(CheatObject))
	{
		if (CheatManagerExtensions.Contains(CheatObject))
		{
			CheatManagerExtensions.Remove(CheatObject);
			CheatObject->RemovedFromCheatManager();
		}
	}
}




