// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PartyMemberSubsystem.h"
#include "System/PDA_PartyMemberData.h"
#include "Structs/CharacterStates.h"
#include "Combat/Party/BattleCharParty_Base.h"
#include "Components/BattleStatsComponent.h"
#include <Kismet/GameplayStatics.h>
#include <DEGameModeBase.h>
#include "NPC/Followers/FollowerManager.h"
#include "Player/DawnsEndPlayerCharacter.h"
#include "CustomLogging/CustomLogCategories.h"
#include "NPC/Followers/FollowerCharacter.h"

bool UPartyMemberSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	Super::ShouldCreateSubsystem(Outer);
	return true;
}

void UPartyMemberSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPartyMemberSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UPartyMemberSubsystem::StoreFieldPosition(FTransform FieldTransform)
{
	PlayerFieldPosition.LastFieldTransform = FieldTransform;
	PlayerFieldPosition.FieldLevelName = (*UGameplayStatics::GetCurrentLevelName(this));

}

void UPartyMemberSubsystem::StoreAreaInfo()
{
	PlayerFieldPosition.AreaInfo = Cast<ADEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->CurrentAreaData;
}

void UPartyMemberSubsystem::StorePartyStates(TArray<class ABattleCharParty_Base*> PartyMembersToSet)
{
	for (int i = 0; i < PartyMembersToSet.Num(); i++)
	{
		for (auto PartyMember : PartyMemberData)
		{
			if (PartyMember.GetName() == PartyMembersToSet[i]->DisplayName)
			{
				PartyMember.CharacterState.ResetValues(PartyMember.GetInitialAttributes(), PartyMembersToSet[i]->BattleStatsComponent->State);
			}
		}
	}


}

void UPartyMemberSubsystem::AddPartyMember(FPartyDataReference PartyMemberToAdd)
{

	//Add New Member To Array
	PartyMemberData.Add(*PartyMemberToAdd.PartyMemberReference.GetRow<FCharacterPartyData>(""));
	InitPartyStats();
	UE_LOGFMT(LogDEPartySubsystemInfo, Display, "Added: {PartyMemberCount}.",
		PartyMemberToAdd.PartyMemberReference.GetRow<FCharacterPartyData>("")->GetName());
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ADawnsEndPlayerCharacter::StaticClass());
	if (FoundActor) {
		Cast<ADawnsEndPlayerCharacter>(FoundActor)->AddFollower();
	}
}

void UPartyMemberSubsystem::InitPartyStats()
{
	for (auto& PartyMember : PartyMemberData)
	{
		PartyMember.CharacterState = FCharacterStates(PartyMember.GetInitialAttributes());
	}
}

FSetElementId UPartyMemberSubsystem::GetElementFromPartySet(FName CharacterToSearch)
{
	FSetElementId CharacterIndex;
	for (auto PartyMember : PartyMemberData)
	{
		if (PartyMember.GetName() == CharacterToSearch)
		{
			CharacterIndex = PartyMemberData.FindId(PartyMember);
			break;
		}
	}
	return CharacterIndex;
}

void UPartyMemberSubsystem::ApplyPartyDataToFollowers()
{
	for (int i = 0; i < Followers.Num(); i++)
	{
		Followers[0]->ApplyMemberData(GetPartyMemberFromSet(i + 1));
	}
}

FCharacterPartyData UPartyMemberSubsystem::GetPartyMemberFromSet(FName CharacterToSearch)
{
	return PartyMemberData[GetElementFromPartySet(CharacterToSearch)];
}

FCharacterPartyData UPartyMemberSubsystem::GetPartyMemberFromSet(int index)
{
	TArray<FCharacterPartyData> PartyDataArray = PartyMemberData.Array();
	return PartyDataArray[index];
}

bool UPartyMemberSubsystem::HasValidIndex(int index)
{
	TArray<FCharacterPartyData> PartyDataArray = PartyMemberData.Array();
	return PartyDataArray.IsValidIndex(index);
}

