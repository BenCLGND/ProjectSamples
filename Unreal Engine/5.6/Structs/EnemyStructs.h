// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/CharacterStruct.h"
#include "Structs/InventoryStructs.h"
#include "Structs/CharacterAttributes.h"
#include "Structs/SkillStructs.h"
#include "EnemyStructs.generated.h"


USTRUCT(BlueprintType)
struct FEnemySkills
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (RowType = "SkillField"))
	FDataTableRowHandle Skill;
};

USTRUCT(BlueprintType)
struct FEnemyDrops
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Slot", meta = (RowType = "InventoryItem"))
	FInventorySlot ItemToDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Chance")
	float Chance = 0;
};


USTRUCT(BlueprintType)
struct FEnemyFields : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	int enemyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	FName enemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	FText enemyDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	class UTexture2D* CTB_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	bool isBoss = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	class UPaperFlipbook* BattleSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	TSubclassOf<class UPaperZDAnimInstance> BattleAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	FCharacterAttributes InitialAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	TSet<FEnemySkills> SkillList;

	UPROPERTY(EditAnywhere, meta = (RowType = "SkillField"))
	FDataTableRowHandle BasicAttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	int DropEXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	int DropAurum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	int DropJobPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	TSet<FEnemyDrops> DroppedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	TSet<FEnemyDrops> StealItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details", meta = (ClampMin = -1, ClampMax = 3), meta = (UIMin = -1, UIMax = 3),
		meta = (ToolTip = "Float Values: 0 = Immunity; -1 = Absorption; 1 = Neutral; \n0.1-0.9 = Resistances; 1.1-3.0 = Weaknesses"))
	TMap<E_ElementalAffinity, float> ElementalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Details")
	TSet<FStatusImmunity> StatusImmunities;

	FEnemyFields() {
		ElementalAttributes.Add(E_ElementalAffinity::NONE, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Fire, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Water, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Earth, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Ice, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Holy, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Dark, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Lightning, 1.0f);
		ElementalAttributes.Add(E_ElementalAffinity::Wind, 1.0f);
	}

};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FEnemyDrops& Thing);
uint32 GetTypeHash(const FEnemyFields& Thing);
uint32 GetTypeHash(const FEnemySkills& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FEnemyDrops& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FEnemyDrops));
}
FORCEINLINE uint32 GetTypeHash(const FEnemyFields& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FEnemyFields));
}
FORCEINLINE uint32 GetTypeHash(const FEnemySkills& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FEnemySkills));
}
#endif
/**
 * 
 */
class DAWNS_END_API UEnemyStructs
{

};
