// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/CharacterStruct.h"

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FLevelStruct& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FLevelStruct));
}
uint32 GetTypeHash(const FCharacterField& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FCharacterField));
}
uint32 GetTypeHash(const FCharacterPartyData& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FCharacterPartyData));
}
uint32 GetTypeHash(const FPartyDataReference& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FPartyDataReference));
}
#endif