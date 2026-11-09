// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/InventoryStructs.h"


#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FInventoryItem& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FInventoryItem));
}

uint32 GetTypeHash(const FInventorySlot& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FInventorySlot));
}

uint32 GetTypeHash(const FBaseItem& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FBaseItem));
}

uint32 GetTypeHash(const FItemField & Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FItemField));
}

uint32 GetTypeHash(const FKeyItemField& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FKeyItemField));
}

uint32 GetTypeHash(const FWeaponField& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FWeaponField));
}

uint32 GetTypeHash(const FArmorField& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FArmorField));
}

uint32 GetTypeHash(const FAccessoryField& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FAccessoryField));
}
uint32 GetTypeHash(const FShopInventorySlot& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FShopInventorySlot));
}
#endif