// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTools/OverworldCheatManagerExtension.h"
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
#include "DayCycle/DayCycleSubsystem.h"
#include "Save/DESaveGameSubsystem.h"
#include "DEGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SuqsProgression.h"
#include "EngineUtils.h"
#include <Engine/ObjectLibrary.h>
#include "System/QuestSubsystem.h"

void UOverworldCheatManagerExtension::PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList)
{
#if UE_WITH_CHEAT_MANAGER

	TArray<UObject*> Results;
	GetObjectsOfClass(UGameInstance::StaticClass(), Results, true, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::None);

	UDawnsEndGameInstance* GameInst = Cast<UDawnsEndGameInstance>(Results[0]);
	//Get GetItem/etc. Autocomplete
	for (auto iter : GameInst->InventoryTables[EInventoryItemType::BaseItem]->GetRowMap())
	{
		FItemField* data = (FItemField*)(iter.Value);

		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveItem %s %i"), *data->RowNameReference.ToString(), 1);

		AutoCompleteCommand.Desc = "Give the specified amount of an item. Can only used be outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#02A4D3");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));
	}
	for (auto iter : GameInst->InventoryTables[EInventoryItemType::Weapon]->GetRowMap())
	{
		FWeaponField* data = (FWeaponField*)(iter.Value);

		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveWeapon %s %i"), *data->RowNameReference.ToString(), 1);

		AutoCompleteCommand.Desc = "Give the specified amount of a Weapon. Can only used be outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#c0c6c7");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));

	}
	for (auto iter : GameInst->InventoryTables[EInventoryItemType::Armor]->GetRowMap())
	{
		FArmorField* data = (FArmorField*)(iter.Value);
		
		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveArmor %s %i"), *data->RowNameReference.ToString(), 1);

		AutoCompleteCommand.Desc = "Give the specified amount of an armor. Can only used be outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#CD7F32");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));
	}
	for (auto iter : GameInst->InventoryTables[EInventoryItemType::Accessory]->GetRowMap())
	{
		FAccessoryField* data = (FAccessoryField*)(iter.Value);

		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveAccessory %s %i"), *data->RowNameReference.ToString(), 1);

		AutoCompleteCommand.Desc = "Give the specified amount of an accessory. Can only be used outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#E0BFB8");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));
	}
	for (auto iter : GameInst->InventoryTables[EInventoryItemType::KeyItem]->GetRowMap())
	{
		FKeyItemField* data = (FKeyItemField*)(iter.Value);
		
		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveKeyItem %s %i"), *data->RowNameReference.ToString(), 1);

		AutoCompleteCommand.Desc = "Gives the specified amount of a key item. Can only be used outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#02A4D3");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));
	}

	//Begin GiveAurum Autocomplete
	int32 intArray[] = { 50, 100, 500, 1000, 5000, 10000, 50000, 1000000 };
	TArray<int> DefaultAurumAmounts;
	DefaultAurumAmounts.Append(intArray);

	for (int i = 0; i < DefaultAurumAmounts.Num(); i++)
	{
		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("GiveAurum %i"), DefaultAurumAmounts[i]);

		AutoCompleteCommand.Desc = "Give the specified amount of Aurum. Can only be used outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#D1B200");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));

	}
	
	//Begin SetTime AutoComplete
	TArray<FString> TimesOfDay;
	TimesOfDay.Add("ETimeOfDay::Dawn");
	TimesOfDay.Add("ETimeOfDay::Noon");
	TimesOfDay.Add("ETimeOfDay::Evening");
	TimesOfDay.Add("ETimeOfDay::Midnight");

	for (int i = 0; i < TimesOfDay.Num(); i++)
	{
		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("SetTime %s"), *TimesOfDay[i]);

		AutoCompleteCommand.Desc = "Set time of day. Can only be used outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#D1B200");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));

	}

	//Add Levels For Demo/Full Game into Autocomplete
	
	TArray<FString> LevelNames;
	LevelNames.Add("LVL_ExplorationZone");
	LevelNames.Add("LVL_VerticalSlice_Colosseum");
	for (int i = 0; i < LevelNames.Num(); i++)
	{
		FAutoCompleteCommand AutoCompleteCommand;

		// The name of cheat must match the start of this
		AutoCompleteCommand.Command = *FString::Printf(TEXT("OpenLevel %s"), *LevelNames[i]);

		AutoCompleteCommand.Desc = "Open Level by Name. Can only be used outside of combat.";

		// You can customize the color or just use the default one
		AutoCompleteCommand.Color = FColor::FromHex("#9299a9");

		// MoveTemp for efficiency
		AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));


	}

	//SUQS Autocomplete
	TArray<UObject*> Assets;
	EngineUtils::FindOrLoadAssetsByPath(TEXT("/Game/Quests"), Assets, EngineUtils::ATL_Regular);
	//Get All DT for the QuestNames and Identifiers
	for (int i = 0; i < Assets.Num(); i++)
	{
		if (Cast<UDataTable>(Assets[i]))
		{
			for (auto iter : Cast<UDataTable>(Assets[i])->GetRowMap())
			{
				FSuqsQuest* data = (FSuqsQuest*)(iter.Value);

				FAutoCompleteCommand AutoCompleteCommand;


				// The name of cheat must match the start of this
				AutoCompleteCommand.Command = *FString::Printf(TEXT("Complete Quest %s "), *data->Identifier.ToString());
				AutoCompleteCommand.Desc = "Completes the specified Quest. Can only be used outside of combat.";

				// You can customize the color or just use the default one
				AutoCompleteCommand.Color = FColor::FromHex("#02A4D3");

				// MoveTemp for efficiency
				AutoCompleteList.Add(MoveTemp(AutoCompleteCommand));

				for (int j = 0; j < data->Objectives.Num(); j++)
				{
					for (int k = 0; k < data->Objectives[j].Tasks.Num(); k++)
					{
						FAutoCompleteCommand AutoCompleteTaskCommand;
						AutoCompleteTaskCommand.Command = *FString::Printf(TEXT("Progress Task %s %s"), *data->Identifier.ToString(), *data->Objectives[j].Tasks[k].Identifier.ToString());
						AutoCompleteTaskCommand.Desc = "Completes the specified Quest's task. Can only be used outside of combat.";
						AutoCompleteTaskCommand.Color = FColor::FromHex("#02A4D3");
						AutoCompleteList.Add(MoveTemp(AutoCompleteTaskCommand));
					}

				}
			}
		}

	}


#endif
}

void UOverworldCheatManagerExtension::AddedToCheatManager_Implementation()
{
	UE_LOGFMT(LogDECheatManager, Display, "Overworld Cheat Extension Added.");
	TArray<UObject*> Results;
	GetObjectsOfClass(UGameInstance::StaticClass(), Results, true, EObjectFlags::RF_ClassDefaultObject, EInternalObjectFlags::None);
	DEGameInst = Cast<UDawnsEndGameInstance>(Results[0]);	
	if (!DEGameInst)
	{
		UE_LOGFMT(LogFailedObject, Fatal, "Game Instance Not Loaded in Overworld Cheat Extension.");
	}
}

void UOverworldCheatManagerExtension::SetTime(ETimeOfDay TimeToSet)
{
	DEGameInst->GetWorld()->GetSubsystem<UDayCycleSubsystem>()->SetTimeOfDay(TimeToSet);
}

void UOverworldCheatManagerExtension::GiveItem(FName ItemToGive, int Quantity)
{
	FDataTableRowHandle ItemRowHandle;
	ItemRowHandle.DataTable = DEGameInst->InventoryTables[EInventoryItemType::BaseItem];
	ItemRowHandle.RowName = ItemToGive;
	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = Quantity;
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddItemToInventory(Item);
}

void UOverworldCheatManagerExtension::GiveWeapon(FName ItemToGive, int Quantity)
{
	FDataTableRowHandle ItemRowHandle;
	ItemRowHandle.DataTable = DEGameInst->InventoryTables[EInventoryItemType::Weapon];
	ItemRowHandle.RowName = ItemToGive;
	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = Quantity;
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddItemToInventory(Item);
}

void UOverworldCheatManagerExtension::GiveAccessory(FName ItemToGive, int Quantity)
{
	FDataTableRowHandle ItemRowHandle;
	ItemRowHandle.DataTable = DEGameInst->InventoryTables[EInventoryItemType::Accessory];
	ItemRowHandle.RowName = ItemToGive;
	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = Quantity;
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddItemToInventory(Item);
}

void UOverworldCheatManagerExtension::GiveArmor(FName ItemToGive, int Quantity)
{
	FDataTableRowHandle ItemRowHandle;
	ItemRowHandle.DataTable = DEGameInst->InventoryTables[EInventoryItemType::Armor];
	ItemRowHandle.RowName = ItemToGive;
	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = Quantity;
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddItemToInventory(Item);
}

void UOverworldCheatManagerExtension::GiveKeyItem(FName ItemToGive, int Quantity)
{
	FDataTableRowHandle ItemRowHandle;
	ItemRowHandle.DataTable = DEGameInst->InventoryTables[EInventoryItemType::KeyItem];
	ItemRowHandle.RowName = ItemToGive;
	FInventorySlot Item;
	Item.TableRow = ItemRowHandle;
	Item.Quantity = Quantity;
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddItemToInventory(Item);
}

void UOverworldCheatManagerExtension::GiveAurum(int AurumToGive)
{
	DEGameInst->GetSubsystem<UInventorySubsystem>()->AddAurum(AurumToGive);
}

void UOverworldCheatManagerExtension::SaveGame(FString SlotName)
{
	DEGameInst->GetSubsystem<UDESaveGameSubsystem>()->CreateNewSaveGame(SlotName);
}

void UOverworldCheatManagerExtension::LoadGame(FString SlotName)
{
	DEGameInst->GetSubsystem<UDESaveGameSubsystem>()->LoadGame(SlotName);
}

void UOverworldCheatManagerExtension::CompleteQuest(FName QuestID)
{
	UE_LOGFMT(LogDECheatManager, Display, "Completed Quest.");
	DEGameInst->GetSubsystem<UQuestSubsystem>()->QuestProgression->CompleteQuest(QuestID);
	DEGameInst->GetSubsystem<UQuestSubsystem>()->GiveQuestRewards(DEGameInst->GetSubsystem<UQuestSubsystem>()->QuestProgression->GetQuest(QuestID));
}

void UOverworldCheatManagerExtension::ProgressQuest(FName QuestID, FName TaskIdentifier)
{
	DEGameInst->GetSubsystem<UQuestSubsystem>()->QuestProgression->CompleteTask(QuestID, TaskIdentifier);
}

void UOverworldCheatManagerExtension::OpenLevel(FName LevelName)
{

	UGameplayStatics::OpenLevel(DEGameInst->GetWorld(), LevelName);

}

void UOverworldCheatManagerExtension::RestartLevel()
{
	ACharacter* Pawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Pawn)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Cast<ADawnsEndPlayerCharacter>(Pawn)->GetLevel());
	}
	
}

//Need specific waypoints per level stored within a map [level->positions]
void UOverworldCheatManagerExtension::TeleportToWaypoint(int WayPoint)
{
	//
}

void UOverworldCheatManagerExtension::CancelEncounters()
{
	DEGameInst->GetGlobalVariables()->EncounterDistance = -1;
}

void UOverworldCheatManagerExtension::EnableEncounters()
{
	DEGameInst->GetGlobalVariables()->EncounterDistance = 3000.0f;
}

void UOverworldCheatManagerExtension::IncreaseCharacterLevel()
{

}

void UOverworldCheatManagerExtension::AddExperience()
{

}

void UOverworldCheatManagerExtension::GiveJP(int JPToGive)
{
	//Need class system setup
}

void UOverworldCheatManagerExtension::SpeedUp(bool SpeedUp)
{

}