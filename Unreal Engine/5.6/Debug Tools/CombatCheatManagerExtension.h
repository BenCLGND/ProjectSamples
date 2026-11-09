// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "System/DE_Enums.h"
#include "Engine/Console.h"
#include "ConsoleSettings.h"
#include "CombatCheatManagerExtension.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_END_API UCombatCheatManagerExtension : public UCheatManagerExtension
{
	GENERATED_BODY()
	
public:
	static void PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList);

	virtual void AddedToCheatManager_Implementation() override;

protected:


private:
	class UDawnsEndGameInstance* DEGameInst;
	class ADEGameModeBase* DEGameMode;

};
