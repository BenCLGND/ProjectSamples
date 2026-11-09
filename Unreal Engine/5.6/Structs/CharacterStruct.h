// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/CharacterAttributes.h"
#include "Structs/CharacterStates.h"
#include "System/DE_Enums.h"
#include "CharacterStruct.generated.h"


USTRUCT(BlueprintType)
struct FLevelStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int XPToLevel = 0;


};

USTRUCT(BlueprintType)
struct FCharacterField : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details")
	class UTexture2D* CTB_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperFlipbook* BattleSprite;

	//map this to weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	TMap<E_WeaponType, class UPaperZDAnimSequence*> DefaultAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperZDAnimSequence* DefaultItemAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperZDAnimSequence* DefaultApproachAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperZDAnimSequence* DefaultReturnAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperZDAnimSequence* DefaultHurtAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Details")
	class UPaperZDAnimSequence* DefaultGuardAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	FCharacterAttributes InitialAttributes;

	UPROPERTY(EditAnywhere, meta = (RowType = "SkillField"), Category = "Character Basic Attack Info")
	FDataTableRowHandle BasicAttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Details", meta = (ClampMin = -1, ClampMax = 3), meta = (UIMin = -1, UIMax = 3),
		meta = (ToolTip = "Float Values: 0 = Immunity; -1 = Absorption; 1 = Neutral; \n 0.1-0.9 = Resistances; 1.1-3.0 = Weaknesses"))
	TMap<E_ElementalAffinity, float> ElementalAttributes;

	FCharacterField() {
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


USTRUCT(BlueprintType)
struct FCharacterPartyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data", meta = (RowType = "CharacterField"),
		meta = (ToolTip = "Set to the correct Row for the character."))
	FDataTableRowHandle CharacterData;

	UPROPERTY(EditAnywhere, Category = "Character Field BP")
	TSubclassOf<class UPaperZDAnimInstance> FieldAnimBP;

	UPROPERTY(EditAnywhere, Category = "Character Battle BP")
	TSubclassOf<class ABattleCharParty_Base> BattleCharacterBlueprint;

	UPROPERTY(VisibleAnywhere, Category = "Character State")
	FCharacterStates CharacterState;

	//Classes
	UPROPERTY(EditAnywhere, Category = "Class Slot", meta = (RowType = "ClassFields"))
	FDataTableRowHandle MainClassRowReference;

	UPROPERTY(EditAnywhere, Category = "Class Slot", meta = (RowType = "ClassFields"))
	FDataTableRowHandle SubClassRowReference;

	UPROPERTY(VisibleAnywhere, Category = "Internal Variables")
	bool isInitialized = false;



	//Gets Initial Attributes from the Character Data 
	//Made to help consolidate some function calls in other objects
	inline FCharacterAttributes GetInitialAttributes()
	{
		return CharacterData.GetRow<FCharacterField>("")->InitialAttributes;
	}

	//Gets Name from the Character Data 
	//Made to help consolidate some function calls in other objects
	inline FName GetName()
	{
		return CharacterData.GetRow<FCharacterField>("")->Name;
	}

	inline bool operator ==(const FCharacterPartyData& other) const {
		return (CharacterData.GetRow<FCharacterField>("")->Name == other.CharacterData.GetRow<FCharacterField>("")->Name);
	}

};

USTRUCT(BlueprintType)
struct FPartyDataReference
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Slot", meta = (RowType = "CharacterPartyData"),
		meta = (ToolTip = "Set this to the enemy that matches the blueprint. \n This must be set."))
	FDataTableRowHandle PartyMemberReference;



};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FLevelStruct& Thing);
uint32 GetTypeHash(const FCharacterField& Thing);
uint32 GetTypeHash(const FCharacterPartyData& Thing);
uint32 GetTypeHash(const FPartyDataReference& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FLevelStruct& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FLevelStruct));
}
FORCEINLINE uint32 GetTypeHash(const FCharacterField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FCharacterField));
}
FORCEINLINE uint32 GetTypeHash(const FCharacterPartyData& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FCharacterPartyData));
}
FORCEINLINE uint32 GetTypeHash(const FPartyDataReference& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FPartyDataReference));
}
#endif

class DAWNS_END_API UCharacterStruct
{

};
