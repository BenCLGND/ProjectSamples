// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "System/DE_Enums.h"
#include "CharacterAttributes.h"
#include "StatusEffectStructs.h"
#include "SkillStructs.h"
#include "InventoryStructs.generated.h"





USTRUCT(BlueprintType)
struct FBaseItem : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details")
	FText ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	int MaxStackSize = 99;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	bool isSellable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Details", 
		meta = (ToolTip = "Used Internally for Inventory System at the moment."))
	EInventoryItemType ItemDataTableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Item Details")
	FName RowNameReference = "";

};

USTRUCT(BlueprintType)
struct FItemField : public FBaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	TSet<E_BaseItemType> ItemTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details", meta = (RowType = "SkillField"))
	FDataTableRowHandle ItemSkill;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details"
		meta = (ToolTip = "Set to true if item can be used in combat."))
	bool isCombatUseable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details"
		meta = (ToolTip = "Set to true if item can be used in the field/world."))
	bool isFieldUseable = false;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	E_ItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	int MarketValue = 0;

	FItemField() {
		ItemDataTableType = EInventoryItemType::BaseItem;
	}

};

USTRUCT(BlueprintType)
struct FKeyItemField : public FBaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Item Details")
	E_KeyItemType ItemType;


	FKeyItemField() {
		ItemDataTableType = EInventoryItemType::KeyItem;
	}
};

USTRUCT(BlueprintType)
struct FWeaponField : public FBaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	E_WeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (ClampMin = 0, ClampMax = 10), meta = (UIMin = 0, UIMax = 10))
	int WeaponTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	E_ItemRarity WeaponRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	int MarketValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	TMap<E_ClassType, int> LevelRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	FCharacterAttributes StatChanges;

	FWeaponField() {
		ItemDataTableType = EInventoryItemType::Weapon;
	}

};

USTRUCT(BlueprintType)
struct FArmorField : public FBaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	E_ArmorZone ArmorZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	E_ArmorHeft ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details", meta = (ClampMin = 0, ClampMax = 10), meta = (UIMin = 0, UIMax = 10))
	int ArmorTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	E_ItemRarity ArmorRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	int MarketValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	TMap<E_ClassType, int> LevelRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	FCharacterAttributes StatChanges;

	FArmorField() {
		ItemDataTableType = EInventoryItemType::Armor;
	}

};

USTRUCT(BlueprintType)
struct FAccessoryField : public FBaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory Details")
	E_ItemRarity AccessoryRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory Details", meta = (ClampMin = 0), meta = (UIMin = 0))
	int MarketValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory Details")
	FCharacterAttributes StatChanges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Details")
	TSet<FStatusImmunity> StatusImmunities;

	//Need To figure out extra columns/variables for the edge cases

	FAccessoryField() {
		ItemDataTableType = EInventoryItemType::Accessory;
	}

};

USTRUCT(BlueprintType)
struct FInventoryItem : public FBaseItem 
{
	GENERATED_BODY()

};



USTRUCT(BlueprintType)
struct FInventorySlot : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (RowType = "BaseItem"))
	FDataTableRowHandle TableRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int Quantity = 0;

	//Operator for equality
	inline bool operator ==(const FInventorySlot& other) const {
		return TableRow.GetRow<FBaseItem>("")->Name == other.TableRow.GetRow<FBaseItem>("")->Name;
	}

	//Helper function to populate item widgets for combat
	inline bool ContainsCombatItem()
	{
		return this->TableRow.GetRow<FItemField>("")->ItemTypes.Contains(E_BaseItemType::CombatItem);
	}

	//Helper Function to check if Skill is Valid
	inline bool IsSkillValid()
	{
		return !(this->TableRow.GetRow<FItemField>("")->ItemSkill.IsNull());
	}

	//Helper Function to get the item skill
	inline FSkillField GetItemSkill()
	{
		return *this->TableRow.GetRow<FItemField>("")->ItemSkill.GetRow<FSkillField>("");
	}

	//Helper function to get Item Name
	inline FName GetItemName()
	{
		return this->TableRow.GetRow<FBaseItem>("")->Name;
	}

	//Helper Function to get Item Icon
	inline TSoftObjectPtr<UTexture2D> GetItemIcon() 
	{
		return this->TableRow.GetRow<FBaseItem>("")->Icon;
	}

	//Helper Function to get Item Description
	inline FText GetItemDescription()
	{
		return this->TableRow.GetRow<FBaseItem>("")->Description;
	}

	//Helper Function to get Item Stack Size
	inline int GetItemStackSize()
	{
		return this->TableRow.GetRow<FBaseItem>("")->MaxStackSize;
	}

};

USTRUCT(BlueprintType)
struct FShopInventorySlot : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item To Buy", meta = (RowType = "BaseItem"))
	FDataTableRowHandle TableRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aurum Needed To Buy")
	int AurumToBuy = 0;

	inline bool operator ==(const FInventorySlot& other) const {
		return TableRow.GetRow<FItemField>("")->Name == other.TableRow.GetRow<FItemField>("")->Name;
	}

	//Helper function to get Item Name
	inline FName GetItemName()
	{
		return this->TableRow.GetRow<FBaseItem>("")->Name;
	}

	//Helper Function to get Item Icon
	inline TSoftObjectPtr<UTexture2D> GetItemIcon()
	{
		return this->TableRow.GetRow<FBaseItem>("")->Icon;
	}

	//Helper Function to get Item Description
	inline FText GetItemDescription()
	{
		return this->TableRow.GetRow<FBaseItem>("")->Description;
	}

	//Helper Function to get Item Stack Size
	inline int GetItemStackSize()
	{
		return this->TableRow.GetRow<FBaseItem>("")->MaxStackSize;
	}
};


#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FInventoryItem& Thing);
uint32 GetTypeHash(const FBaseItem& Thing);
uint32 GetTypeHash(const FInventorySlot& Thing);
uint32 GetTypeHash(const FItemField& Thing);
uint32 GetTypeHash(const FKeyItemField& Thing);
uint32 GetTypeHash(const FWeaponField& Thing);
uint32 GetTypeHash(const FArmorField& Thing);
uint32 GetTypeHash(const FAccessoryField& Thing);
uint32 GetTypeHash(const FShopInventorySlot& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FInventoryItem& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FInventoryItem));
}
FORCEINLINE uint32 GetTypeHash(const FBaseItem& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FBaseItem));
}
FORCEINLINE uint32 GetTypeHash(const FInventorySlot& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FInventorySlot));
}
FORCEINLINE uint32 GetTypeHash(const FItemField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FItemField));
}
FORCEINLINE uint32 GetTypeHash(const FKeyItemField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FKeyItemField));
}
FORCEINLINE uint32 GetTypeHash(const FWeaponField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FWeaponField));
}
FORCEINLINE uint32 GetTypeHash(const FArmorField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FArmorField));
}
FORCEINLINE uint32 GetTypeHash(const FAccessoryField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FAccessoryField));
}
FORCEINLINE uint32 GetTypeHash(const FShopInventorySlot& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FShopInventorySlot));
}
#endif

class DAWNS_END_API InventoryStructs
{
};
