// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/EnemyStructs.h"

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FEnemyDrops& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FEnemyDrops));
}

uint32 GetTypeHash(const FEnemyFields& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FEnemyFields));
}

uint32 GetTypeHash(const FEnemySkills& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FEnemySkills));
}
#endif
