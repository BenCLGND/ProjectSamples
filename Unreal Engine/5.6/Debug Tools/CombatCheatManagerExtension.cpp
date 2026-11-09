// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTools/CombatCheatManagerExtension.h"
#include "Engine/Console.h"
#include "System/DE_Enums.h"
#include <System/DawnsEndGameInstance.h>
#include "System/InventorySubsystem.h"
#include "Structs/InventoryStructs.h"
#include "System/PartyMemberSubsystem.h"
#include "System/EquipmentSubsystem.h"
#include "System/ClassSubsystem.h"
#include "ConsoleSettings.h"
#include "CustomLogging/CustomLogCategories.h"

void UCombatCheatManagerExtension::PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList)
{
#if UE_WITH_CHEAT_MANAGER



#endif
}

void UCombatCheatManagerExtension::AddedToCheatManager_Implementation()
{
	UE_LOGFMT(LogDECheatManager, Display, "Combat Cheat Extension Added.");
	TArray<UObject*> Results;
	GetObjectsOfClass(UGameInstance::StaticClass(), Results, true, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::None);
	DEGameInst = Cast<UDawnsEndGameInstance>(Results[0]);
	if (!DEGameInst)
	{
		UE_LOGFMT(LogFailedObject, Fatal, "Game Instance Not Loaded in Overworld Cheat Extension.");
	}
}