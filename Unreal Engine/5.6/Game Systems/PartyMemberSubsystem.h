// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/CharacterStruct.h"
#include "PartyMemberSubsystem.generated.h"


USTRUCT()
struct FPlayerFieldPosition {
	GENERATED_BODY()

	UPROPERTY(SaveGame, VisibleAnywhere)
	//Stores player's last field location
	FTransform LastFieldTransform;

	UPROPERTY(SaveGame, VisibleAnywhere)
	//Stores Field Level Name to Load the Field Player into
	FName FieldLevelName = "None";

	UPROPERTY(SaveGame, VisibleAnywhere)
	class UPDA_AreaData* AreaInfo;

};
/**
 * 
 */
UCLASS()
class DAWNS_END_API UPartyMemberSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Override the virtual functions of USubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(SaveGame, VisibleAnywhere, SaveGame)
	struct FPlayerFieldPosition PlayerFieldPosition;

	//Order of this array decides who runs in the field and the main party in battle
	UPROPERTY(SaveGame, EditAnywhere, SaveGame)
	TSet<FCharacterPartyData> PartyMemberData;

	UPROPERTY(SaveGame)
	TSoftObjectPtr<UWorld> LevelToSave;

	//Follower Manager Variables For Persistence
	UPROPERTY(SaveGame)
	int FollowerCount = 0;

	UPROPERTY(SaveGame)
	FVector LastTrackedPosition;

	//UPROPERTY(SaveGame)
	//TArray<FVector> FollowerPositions;

	UPROPERTY(SaveGame)
	TArray<class AFollowerCharacter*> Followers;

	UFUNCTION(BlueprintCallable)
	void StoreFieldPosition(FTransform FieldTransform);

	UFUNCTION(BlueprintCallable)
	void StoreAreaInfo();

	//Stores States to ensure the values reflect both in field and in the next battle
	UFUNCTION(BlueprintCallable)
	void StorePartyStates(TArray<class ABattleCharParty_Base*> PartyMembersToSet);

	//Adds PartyMember to the Array
	UFUNCTION()
	void AddPartyMember(FPartyDataReference PartyMemberToAdd);

	UFUNCTION()
	void InitPartyStats();

	UFUNCTION()
	FCharacterPartyData GetPartyMemberFromSet(FName CharacterToSearch);

	FCharacterPartyData GetPartyMemberFromSet(int index);

	UFUNCTION()
	bool HasValidIndex(int index);

	UFUNCTION()
	void ApplyPartyDataToFollowers();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table References", meta = (RequiredAssetDataTags = "RowStructure=/Script/Dawns_End.LevelStruct"))
	UDataTable* CharacterLevelTable;

private:
	FSetElementId GetElementFromPartySet(FName CharacterToSearch);

};
