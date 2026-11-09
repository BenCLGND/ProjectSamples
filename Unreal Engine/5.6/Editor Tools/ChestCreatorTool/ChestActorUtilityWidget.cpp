// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTools/ChestActorUtilityWidget.h"
#include "Interactables/TreasureChestActor.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "Structs/InventoryStructs.h"
#include "Engine/DataTable.h"
#include "CustomTools/DEUnrealEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "CustomLogging/CustomLogCategories.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/VerticalBox.h"
#include "CustomTools/ContainerUtilityItemBoxWidget.h"
#include <Kismet/GameplayStatics.h>


void UChestActorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EditorSubsystem = Cast<UDEUnrealEditorSubsystem>(GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>());
	ActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	FurnitureTypeBox->SetVisibility(ESlateVisibility::Collapsed);
	ChestTypeBox->SetVisibility(ESlateVisibility::Collapsed);
	ContainerKey->OnSelectionChanged.AddUniqueDynamic(this, &UChestActorUtilityWidget::CheckContainerKey);
	CreateChestButton->OnClicked.AddDynamic(this, &UChestActorUtilityWidget::CreateContainerActor);
	AddItemButton->OnClicked.AddDynamic(this, &UChestActorUtilityWidget::AddItemWidget);
	RemoveItemButton->OnClicked.AddDynamic(this, &UChestActorUtilityWidget::RemoveItemWidget);
	CreateChestKey();
	CreateContainerKey();
}

void UChestActorUtilityWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UChestActorUtilityWidget::CreateContainerActor()
{
	int AurumToAdd = 0;
	if (ContainerKey->GetSelectedOption().Equals("None"))
	{
		return;
	}
	//Aurum Check
	if (FCString::Atoi(*AurumAmountText->GetText().ToString()) <= 0)
	{
		AurumToAdd = 0;
	}
	else
	{
		AurumToAdd = FCString::Atoi(*AurumAmountText->GetText().ToString());
	}

	if (ContainerKey->GetSelectedOption().Equals("Chest") && !ChestKey->GetSelectedOption().Equals("None"))
	{
		CreateChestActor(TrunkMeshMap[ChestKey->GetSelectedOption()], LidMeshMap[ChestKey->GetSelectedOption()], GetItem(), AurumToAdd);
	}
	else if (ContainerKey->GetSelectedOption().Equals("Furniture"))
	{
		//CreateFurnitureActor();
	}

}

void UChestActorUtilityWidget::AddItemWidget()
{
	UContainerUtilityItemBoxWidget* ItemBoxToAdd = CreateWidget<UContainerUtilityItemBoxWidget>(this, ItemBoxWidgetReference);
	ItemToAddBox->AddChild(ItemBoxToAdd);
}

void UChestActorUtilityWidget::RemoveItemWidget()
{
	if (ItemToAddBox->GetChildrenCount() != 0)
	{
		ItemToAddBox->RemoveChildAt(ItemToAddBox->GetChildrenCount() - 1);
	}
	
}

void UChestActorUtilityWidget::CheckContainerKey(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Chest")
	{
		FurnitureTypeBox->SetVisibility(ESlateVisibility::Collapsed);
		ChestTypeBox->SetVisibility(ESlateVisibility::Visible);

	}
	else if (SelectedItem == "Furniture")
	{
		FurnitureTypeBox->SetVisibility(ESlateVisibility::Visible);
		ChestTypeBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (SelectedItem == "None")
	{
		FurnitureTypeBox->SetVisibility(ESlateVisibility::Collapsed);
		ChestTypeBox->SetVisibility(ESlateVisibility::Collapsed);

	}
	else
	{
		FurnitureTypeBox->SetVisibility(ESlateVisibility::Collapsed);
		ChestTypeBox->SetVisibility(ESlateVisibility::Collapsed);

	}

}





void UChestActorUtilityWidget::CreateChestKey()
{
	ChestKey->AddOption("None");
	for (auto iter : TrunkMeshMap) {
		ChestKey->AddOption(iter.Key);
	}
	ChestKey->SetSelectedOption("None");
}

void UChestActorUtilityWidget::CreateContainerKey()
{
	ContainerKey->AddOption("None");
	ContainerKey->AddOption("Chest");
	ContainerKey->AddOption("Furniture");
	ContainerKey->SetSelectedOption("None");
}

TSet<FInventorySlot> UChestActorUtilityWidget::GetItem()
{
	TSet<FInventorySlot> ItemsToReturn;
	if (ItemToAddBox->GetChildrenCount() == 0)
	{
		return ItemsToReturn;
	}
	for (int i = 0; i < ItemToAddBox->GetChildrenCount(); i++)
	{
		UContainerUtilityItemBoxWidget* ChildToCheck = Cast<UContainerUtilityItemBoxWidget>(ItemToAddBox->GetChildAt(i));
		if (!ChildToCheck)
		{
			UE_LOGFMT(LogTemp, Error, "ItemWidget Not Valid!");
			continue;
		}
		if (ItemsToReturn.Contains(ChildToCheck->GetItemFromKeys()))
		{
			ItemsToReturn.Find(ChildToCheck->GetItemFromKeys())->Quantity += ChildToCheck->GetItemFromKeys().Quantity;
		}
		else {
			ItemsToReturn.Add(ChildToCheck->GetItemFromKeys());
		}
	}

	return ItemsToReturn;
}

void UChestActorUtilityWidget::CreateChestActor(UStaticMesh* TrunkMeshToSet, UStaticMesh* LidMeshToSet, TSet<FInventorySlot> ItemsToAdd, int Aurum)
{
	UWorld* World = nullptr;
	World = GEditor->GetEditorWorldContext().World();
	FVector Location;
	FRotator Rotation;
	EditorSubsystem->GetLevelViewportCameraInfo(Location, Rotation);
	FHitResult Hit;
	FVector TraceEnd = Location + UKismetMathLibrary::GetForwardVector(Rotation) * 1000.0f;
	
	if (PerformLineTraceForSpawn(Location, TraceEnd, Hit))
	{
		ATreasureChestActor* CreatedActor = Cast<ATreasureChestActor>(ActorSubsystem->SpawnActorFromClass(TreasureChestBlueprint,
			Hit.ImpactPoint, FRotator(0), false));
		CreatedActor->CreateFromUtility(TrunkMeshToSet, LidMeshToSet, ItemsToAdd, Aurum);
	}
	else
	{
		ATreasureChestActor* CreatedActor = Cast<ATreasureChestActor>(ActorSubsystem->SpawnActorFromClass(TreasureChestBlueprint,
			 TraceEnd, FRotator(0), false));
		CreatedActor->CreateFromUtility(TrunkMeshToSet, LidMeshToSet, ItemsToAdd, Aurum);
	}


	
}



void UChestActorUtilityWidget::CreateFurnitureActor(UStaticMesh* MeshToSet, FInventorySlot ItemToAdd, int Aurum)
{
	UWorld* World = nullptr;
	World = GEditor->GetEditorWorldContext().World();
	FVector Location;
	FRotator Rotation;
	EditorSubsystem->GetLevelViewportCameraInfo(Location, Rotation);
}

bool UChestActorUtilityWidget::PerformLineTraceForSpawn(FVector CameraLocation, FVector& TraceEnd, FHitResult& Hit)
{

	// We set up a line trace from our current location to a point 1000cm ahead of us
	FVector TraceStart = CameraLocation;

	// You can use FCollisionQueryParams to further configure the query
	FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor();

	// To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
	// UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic, QueryParams);

	// You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	//UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		//UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
		return true;
		
	}
	else {
		//UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
		return false;
	}
}
