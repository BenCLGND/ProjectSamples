// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InventorySubsystem.h"
#include "Structs/InventoryStructs.h"
#include "CustomLogging/CustomLogCategories.h"
#include "System/QuestSubsystem.h"
#include "System/DawnsEndGameInstance.h"

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	Super::ShouldCreateSubsystem(Outer);
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GameInst = Cast<UDawnsEndGameInstance>(GetGameInstance());
}

void UInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();

}

int UInventorySubsystem::UseItemFromInventory(FInventorySlot ItemToUse)
{

	if (Inventory.Contains(ItemToUse))
	{
		Inventory.Find(ItemToUse)->Quantity -= 1;
		UE_LOGFMT(LogItemInfo, Display, "Player Used {Name} x {Amount}", ItemToUse.GetItemName(), ItemToUse.Quantity);
		return Inventory.Find(ItemToUse)->Quantity;
	}
	else {
		return 0;
	}

}

void UInventorySubsystem::AddItemToInventory(FInventorySlot ItemToAdd)
{
	
	if (Inventory.Contains(ItemToAdd))
	{
		Inventory.Find(ItemToAdd)->Quantity += ItemToAdd.Quantity;
	}
	else {
		Inventory.Add(ItemToAdd);
	}
	UE_LOGFMT(LogItemInfo, Display, "Player Received {Name} x {Amount}", ItemToAdd.GetItemName(), ItemToAdd.Quantity);
}

void UInventorySubsystem::AddAurum(int ValueToAdd)
{
	AurumAmount += ValueToAdd;
	if (AurumAmount >= INT32_MAX)
	{
		AurumAmount = INT32_MAX;
	}
}

bool UInventorySubsystem::SubtractAurum(int ValueToSubtract)
{
	int TempValue = AurumAmount;
	TempValue -= ValueToSubtract;

	if (TempValue >= 0) {
		AurumAmount -= ValueToSubtract;
		return true;
	}
	else
	{
		return false;
	}
}