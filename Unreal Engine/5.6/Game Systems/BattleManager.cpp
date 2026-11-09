// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BattleManager.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/ShakerActorComponent.h"
#include <System/DawnsEndGameInstance.h>
#include "System/UtilFunctionLibrary.h"
#include "Components/BattleStatsComponent.h"
#include "System/PDA_AreaData.h"
#include "System/PDA_PartyMemberData.h"
#include "Widgets/ActionMenuWidget.h"
#include "Widgets/ItemListWidget.h"
#include "Player/DawnsEndPlayerController.h"
#include "CustomLogging/CustomLogCategories.h"
#include "System/DE_Enums.h"
#include "System/InventorySubsystem.h"
#include "System/PartyMemberSubsystem.h"
#include "DebugTools/OverworldCheatManagerExtension.h"
#include <DayCycle/DayCycleSubsystem.h>
#include "System/PartyMemberSubsystem.h"



// Sets default values
ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = true;

	FRotator NewRotation = FRotator(0, -30, -90);
	FQuat QuatRotation = FQuat(NewRotation);
	this->AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	//Create Camera Component
	SetUpCamera();
	
	//Create Scene Component
	ActiveLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Active Location"));
	ActiveLocation->SetupAttachment(RootComponent);
	ActiveLocation->AddLocalOffset(FVector(-125, 0, 0), false, 0, ETeleportType::None);

	//Create Timeline Component
	SetUpTimelines();


	//Default Positions and Rotations if not set
	PartyMemberLocations.Add(FVector(250, -300, 0));
	PartyMemberLocations.Add(FVector(350, -100, 0));
	PartyMemberLocations.Add(FVector(450, 100, 0));
	PartyMemberLocations.Add(FVector(550, 300, 0));
	EnemyMemberLocations.Add(FVector(-600, -150, 0));
	EnemyMemberLocations.Add(FVector(-450, 150, 0));
	EnemyMemberLocations.Add(FVector(-300, -150, 0));
	EnemyMemberLocations.Add(FVector(-150, 150, 0));
	PartyRotation = FRotator(0.0f, 180.0f, 0.0f);
	EnemyRotation = FRotator(0.0f, 0.0f, 0.0f);
	ZAxisOffset = FVector(0, 0, 80);
	



}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UDayCycleSubsystem>()->PauseTimer(true);
	DEController = Cast<ADawnsEndPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	DEController->SetIsBattle(true);
	GlobalVariables = Cast<UDawnsEndGameInstance>(GetGameInstance())->GetGlobalVariables();
	if (!GlobalVariables) {
		printFStringColor("Global Failed", Red));
	}

	this->EnableInput(DEController);
	SetUpIMC();

	MoveToTimelineComponent->AddInterpFloat(MoveToActivePositionCurve, MoveToProgressUpdate);
	MoveFromTimelineComponent->AddInterpFloat(MoveToActivePositionCurve, MoveFromProgressUpdate);
	AnimChromaticComponent->AddInterpFloat(AnimChromaticCurve, AnimChromaticProgressUpdate);
	GameInstance = Cast<UDawnsEndGameInstance>(GetGameInstance());

	if (IsValid(DEController)) {
		DEController->Possess(this);
		DEController->InitBattleWidget(this);
		DEController->RemoveOverworldCheats();
		DEController->AddCombatCheats();
	}
	else {
		UE_LOGFMT(LogFailedObject, Fatal, "Dawn's End Controller failed to load.");
	}
	BeginBattle_Implementation();
	
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABattleManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(AcceptAction, ETriggerEvent::Started, this, &ABattleManager::Accept);
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Started, this, &ABattleManager::Back);
		EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Started, this, &ABattleManager::Left);
		EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Started, this, &ABattleManager::Right);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABattleManager::PauseGame);
	}
	else
	{
		UE_LOGFMT(LogMissingReference, Fatal, "Please set the Player Input Component.");
	}

}

//Initializes Arrays, Spawns Actors, and Begins the CTB
void ABattleManager::BeginBattle_Implementation()
{
	SpawnActors();
	GetBattleMembers();
	DecideTurnOrder();
}

//Starts the turn logic
void ABattleManager::StartTurn_Implementation()
{
	//TODO: Make Status Ailments Proc
	//Make sure that a turn can be taken
	if (!PartyMembersAlive.IsEmpty() && !EnemyMembersAlive.IsEmpty()) {

		//If Active Character is a party member, begin player actions
		if (ActiveCharacter->isPartyMember) {
			//Moves Active Party Member to the Active Location to signify whose turn is active
			StartMoveToActivePosition();
		}
		//Allow Enemy to take turn
		else {
			EnemyTurn();
		}
	}
	else {

	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (ActiveCharacter != NULL) ? ActiveCharacter->GetName() : FString(TEXT("None")));

	
}

//Concludes the turn, so as to begin the next one
void ABattleManager::ConcludeTurn_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Turn Ended!")));
	//ActiveCharacter->HideBattlePortrait();
	if (CanEndBattle)
	{
		return;
	}

	//Subtract CTB value
	ActiveCharacter->SubtractCTBValue();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::SanitizeFloat(ActiveCharacter->CTBValue) + " " + ActiveCharacter->GetName());

	//Make sure to reset Target Characters Array
	TargetCharacters.Empty();

	//Start another turn if the active character still meets the CTB Threshold
	if (ActiveCharacter->CTBValue >= GameInstance->GetGlobalVariables()->CTBCapValue) {
		StartTurn_Implementation();
	}
	//Find another Active character
	else {
		ActiveCharacter = nullptr;
		UpdateTurnOrder();
	}
}

//Ends the battle and returns the player to the appropriate level
void ABattleManager::EndBattle_Implementation() 
{
	if (LostBattle)
	{
		//Load Main Menu here

	}
	else 
	{
		DEController->AddOverworldCheats();
		DEController->RemoveCombatCheats();
		DEController->SetIsBattle(false);
		ActiveCharacter->StopAnimations();
		if (GameInstance) {

			//Overwrite party states
			GameInstance->GetPartyMemberSubsystem()->StorePartyStates(PartyMembers);

			//Hide the battle widget
			DEController->HideBattleWidget();

			//Load the player into the previous location
			UGameplayStatics::OpenLevel(this, GameInstance->GetPartyMemberSubsystem()->PlayerFieldPosition.FieldLevelName);
			GetWorld()->GetSubsystem<UDayCycleSubsystem>()->PauseTimer(false);
			UE_LOGFMT(LogCombatInfo, Display, "Battle Ended");

		}
		else {
			//UE_LOG(LogMissingReference, Fatal, TEXT"Please set the GameInstance Reference.");
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Game Instance Failed")));
		}
	}

}

//Called when a target is being selected: makes the pointer widget appear
void ABattleManager::StartTargetPickMode(int SkillIndex)
{
	CurrentSkillStruct = ActiveCharacter->GetSkillByIndex(SkillIndex);
	//Reset the array 
	TargetCharacters.Empty();
	switch (CurrentSkillStruct.TargetStyle)
	{
	case E_ENUMTargetStyle::SingleTarget:
		TargetCharacters.Add(GetTargetTeam()[0]);
		break;
	case E_ENUMTargetStyle::Everyone:
		TargetCharacters = GetTargetTeam();
		break;
	case E_ENUMTargetStyle::EntireTeam:
		switch (CurrentSkillStruct.TargetGroup)
		{
		case E_ENUMTargetGroup::Party:
			EnemyTeamSelected = false;
			HighlightTeam();
			break;
		case E_ENUMTargetGroup::Enemies:
			EnemyTeamSelected = true;
			HighlightTeam();
			break;
		default:
			break;
		}
		break;
	case E_ENUMTargetStyle::Self:
		TargetCharacters[0] = ActiveCharacter;
		break;
	default:
		break;
	}
	for (int i = 0; i < TargetCharacters.Num(); i++)
	{
		TargetCharacters[i]->SetPointerVisibilty(true);
	}
	
	DEController->CreatedBattleWidget->SetActionMenuVisible(false);
	DEController->DeactivateUIControls();
	CanBattleNav = true;
}

//Override of the previous function for special instances
void ABattleManager::StartTargetPickMode(FSkillField SkillTableData)
{
	CurrentSkillStruct = SkillTableData;
	TargetCharacters.Empty();
	switch (CurrentSkillStruct.TargetStyle)
	{
	case E_ENUMTargetStyle::SingleTarget:
		TargetCharacters.Add(GetTargetTeam()[0]);
		break;
	case E_ENUMTargetStyle::Everyone:
		TargetCharacters = GetTargetTeam();
		break;
	case E_ENUMTargetStyle::EntireTeam:
		switch (CurrentSkillStruct.TargetGroup)
		{
		case E_ENUMTargetGroup::Party:
			EnemyTeamSelected = false;
			HighlightTeam();
			break;
		case E_ENUMTargetGroup::Enemies:
			EnemyTeamSelected = true;
			HighlightTeam();
			break;
		default:
			break;
		}
		break;
	case E_ENUMTargetStyle::Self:
		TargetCharacters[0] = ActiveCharacter;
		break;
	default:
		break;
	}
	for (int i = 0; i < TargetCharacters.Num(); i++)
	{
		TargetCharacters[i]->SetPointerVisibilty(true);
	}
	DEController->CreatedBattleWidget->SetActionMenuVisible(false);
	DEController->DeactivateUIControls();
	CanBattleNav = true;
}

//Hides the pointer widgets
void ABattleManager::EndTargetPickMode()
{
	for (int i = 0; i < TargetCharacters.Num(); i++)
	{
		TargetCharacters[i]->SetPointerVisibilty(false);
	}
	TargetIndex = 0;
	CanBattleNav = false;
	
}

//Allows the player to move the target pointer for single target skills
void ABattleManager::MoveTargetPointer(bool MoveRight)
{
	if (CurrentSkillStruct.TargetStyle == E_ENUMTargetStyle::SingleTarget) {
		GetTargetTeam()[TargetIndex]->SetPointerVisibilty(false);
		if (MoveRight) {
			//printFStringColor("Moving Pointer", Orange));
			TargetIndex++;
			if (TargetIndex >= GetTargetTeam().Num()) {
				TargetIndex = 0;
			}

		}
		else {
			//printFStringColor("Moving Pointer", Orange));
			TargetIndex--;
			if (TargetIndex < 0) {
				TargetIndex = GetTargetTeam().Num() - 1;
			}


		}
		TargetCharacters[0] = GetTargetTeam()[TargetIndex];
		GetTargetTeam()[TargetIndex]->SetPointerVisibilty(true);
	}
	else if (CurrentSkillStruct.TargetStyle == E_ENUMTargetStyle::EntireTeam) 
	{
		HighlightTeam();
		if (MoveRight) {
			EnemyTeamSelected = EnemyTeamSelected ? false : true;
		}
		else {
			EnemyTeamSelected = EnemyTeamSelected ? false : true;
		}
		HighlightTeam();
	}
}

//Gets appropriate array to target. 
TArray<ABattleChar_BASE*> ABattleManager::GetTargetTeam()
{
	TArray<ABattleChar_BASE*> TeamToReturn;
	switch (CurrentSkillStruct.TargetGroup)
	{
	case E_ENUMTargetGroup::Party:
		TeamToReturn.Append(PartyMembersAlive);
		if (CurrentSkillStruct.ContainsResurrection()) 
		{
			TeamToReturn.Append(PartyMembersDead);
		}
		TeamToReturn.Append(EnemyMembersAlive);
		break;
	case E_ENUMTargetGroup::Enemies:
		TeamToReturn.Append(EnemyMembersAlive);
		TeamToReturn.Append(PartyMembersAlive);
		break;
	default:
		break;
	}
	return TeamToReturn;
}

//Called to begin calculations for damage and healing
void ABattleManager::ApplyBattleEffect(UBattleStatsComponent* InstigatorStats)
{
	for (int i = 0; i < TargetCharacters.Num(); i++)
	{
		//TargetCharacters[i]->ReceiveBattleEffect(InstigatorStats, CurrentSkillData);
		TargetCharacters[i]->ReceiveBattleEffect(InstigatorStats, CurrentSkillStruct);
	}


	//DEController->CreatedBattleWidget->ShowNotificationWithDelay(CurrentSkillData->SkillName, 1.5f);
	DEController->CreatedBattleWidget->ShowNotificationWithDelay(CurrentSkillStruct.SkillName, 1.5f);
	
	StartAnimChromatic();
}

//Called to start Enemy's Turn
//TODO: Add Enemy AI logic in here
void ABattleManager::EnemyTurn()
{
	//Choose enemy skill
	UpdateEnemySkill();

	//Decide Attack Target
	DecideTargetPlayer();

	//Begin Animation for attack
	ActiveCharacter->PlayAttackAnim_Implementation(CurrentSkillStruct);
	
}

//Chooses Party Member(s) to attack
void ABattleManager::DecideTargetPlayer()
{
	switch (CurrentSkillStruct.TargetStyle)
	{
		case E_ENUMTargetStyle::SingleTarget:
			TargetCharacters.Add(PartyMembersAlive[FMath::RandRange(0, PartyMembersAlive.Num()-1)]);
			break;
		case E_ENUMTargetStyle::EntireTeam:
			TargetCharacters.Append(PartyMembersAlive);
			break;
		default:
			break;
	}

	
}

//Ends the shake of party members and ends enemy turn
void ABattleManager::EnemyFinishedAttack()
{
	ActiveCharacter->ExecuteBattleEffect();
	DELAY(GlobalVariables->AfterAttackDelay, { 
		for (int i = 0; i < TargetCharacters.Num(); i++)
		{
		TargetCharacters[i]->ShakerComponent->EndShake();
		}
		ConcludeTurn_Implementation(); })
	
}

//Sets the Active Character into the isDefending state
void ABattleManager::StartDefending()
{
	ActiveCharacter->SetDefending(true);
	DELAY(1.1f, {StartMoveToStartingPosition(); })
	
}

//Remove KOed characters from the appropriate array
void ABattleManager::UpdateAliveMembers(ABattleChar_BASE* CharacterToAdjust, bool isDead)
{
	if (isDead) {
		RemoveFromTurnOrder(CharacterToAdjust);
		if (CharacterToAdjust->isPartyMember) {
			PartyMembersDead.Add(Cast<class ABattleCharParty_Base>(CharacterToAdjust));
			PartyMembersAlive.Remove(Cast<class ABattleCharParty_Base>(CharacterToAdjust));
			//If party is defeated, end battle
			if (PartyMembersAlive.IsEmpty()) {
				BattleResult(EEnumBattleOutcome::PartyLost);
			}
		}
		else {
			EnemyMembersDead.Add(Cast<class ABattleCharEnemy_BASE>(CharacterToAdjust));
			EnemyMembersAlive.Remove(Cast<class ABattleCharEnemy_BASE>(CharacterToAdjust));
			//Add total xp for possible victory
			XPTotal += Cast<class ABattleCharEnemy_BASE>(CharacterToAdjust)->DropEXP;
			//if enemies are defeated, end battle 
			if (EnemyMembersAlive.IsEmpty()) {
				BattleResult(EEnumBattleOutcome::PartyWon);
			}
		}
	}
	else {
		AddToTurnOrder(CharacterToAdjust);
		if (CharacterToAdjust->isPartyMember)
		{
			PartyMembersAlive.Add(Cast<class ABattleCharParty_Base>(CharacterToAdjust));
			PartyMembersDead.Remove(Cast<class ABattleCharParty_Base>(CharacterToAdjust));
		}
		else {
			EnemyMembersAlive.Add(Cast<class ABattleCharEnemy_BASE>(CharacterToAdjust));
			EnemyMembersDead.Remove(Cast<class ABattleCharEnemy_BASE>(CharacterToAdjust));
		}
	}

}

//Removes character from turn order upon defeat
void ABattleManager::RemoveFromTurnOrder(ABattleChar_BASE* CharacterToRemove)
{
	
	
	CharacterToRemove->CTBValue = 0;
	TurnOrder.Sort(CTBSortPredicate);
	DEController->CreatedBattleWidget->SortTurnPortraits(TurnOrder);
	TurnOrder.Remove(CharacterToRemove);
	
}

void ABattleManager::AddToTurnOrder(ABattleChar_BASE* CharacterToAdd)
{
	TurnOrder.Add(CharacterToAdd);
}

//Called when the battle ends to do the appropriate condition
void ABattleManager::BattleResult(EEnumBattleOutcome BattleOutcome)
{
	DEController->CreatedBattleWidget->CollapsePartyStatBox();
	switch (BattleOutcome) {
	case EEnumBattleOutcome::PartyWon:
		//Get items to add to inventory
		DecideDrops();
		//Give party their xp
		YieldPartyXP();
		//Show info for player
		DEController->CreatedBattleWidget->ShowBattleResultScreen(ItemsForWidget, AurumToAdd, XPDataArray);
		//Really big delay to not cause an error, can be shortened
		//DELAY(0.0f, { CanEndBattle = true; });
		//EndBattle_Implementation();
		CanEndBattle = true;
		break;
	case EEnumBattleOutcome::PartyLost:
		DEController->CreatedBattleWidget->ShowNotification(FText::FromString("The Party is Defeated..."));
		//TODO: Change to load game over screen or main menu
		LostBattle = true;
		DELAY(1.0f, { CanEndBattle = true; });
		
		//DELAY(5.0f, { EndBattle_Implementation(); });
		//CanEndBattle = true;
		break;
	case EEnumBattleOutcome::PartyFled:
		DEController->CreatedBattleWidget->SetActionMenuVisible(false);
		DEController->CreatedBattleWidget->ShowNotification(FText::FromString("The Party Fled"));
		DELAY(GlobalVariables->FleeDelay, { EndBattle_Implementation(); });
		break;
	default:
		break;
	}

	
}

//Attempts to flee the battle
void ABattleManager::AttemptFlee()
{
	
	//If successful, end the battle
	if (FMath::RandRange(0, 100) < GlobalVariables->FleeChance) {
		BattleResult(EEnumBattleOutcome::PartyFled);
	}
	//Skip Active Player's Turn
	else {
		DEController->CreatedBattleWidget->ShowNotificationWithDelay(FText::FromString("Failed to Flee"), 2.0f);
		DEController->CreatedBattleWidget->SetActionMenuVisible(false);
		DELAY(GlobalVariables->FleeDelay, { SkipPartyTurn(); StartMoveToStartingPosition(); });
		
	}
	
}

//Used to skip a turn if flee fails
void ABattleManager::SkipPartyTurn()
{
	ActiveCharacter->SubtractCTBValue();
}

//Sets CurrentSkillData to use its basic attack
//TODO: Edit for other skills
void ABattleManager::UpdateEnemySkill()
{
	//CurrentSkillData->ConstructSkillFromTable(*ActiveCharacter->BattleCharacterData->BasicAttackData.GetRow<FSkillField>(""));
	CurrentSkillStruct = *ActiveCharacter->BasicAttackData.GetRow<FSkillField>("");
}



// Gets the drops from enemies and decides if they are rewarded to the player
void ABattleManager::DecideDrops()
{
	float randomFloat = FMath::RandRange(0.0f, 100.0f);
	for (int i = 0; i < EnemyMembers.Num(); i++)
	{
		//Add currency to be rewarded
		AurumToAdd += EnemyMembers[i]->DropAurum;
		for (auto& Elem : EnemyMembers[i]->DroppedItems) 
		{
			if (randomFloat <= Elem.Chance) {
				GameInstance->GetInventorySubsystem()->AddItemToInventory(Elem.ItemToDrop);
				AddItemToArray(Elem.ItemToDrop);

			}
		}
	}
}

//Calls the battlestatscomponent to do the XP calculations
void ABattleManager::YieldPartyXP()
{
	for (int i = 0; i < PartyMembersAlive.Num(); i++)
	{
		//Create this struct to pass into the BattleStats
		FLevelDisplayData LVLData;
		LVLData.CharacterName = FText::FromName(PartyMembersAlive[i]->DisplayName);
		LVLData.StartXP = PartyMembersAlive[i]->BattleStatsComponent->State.XP;
		LVLData.StartLevel = PartyMembersAlive[i]->BattleStatsComponent->State.Level;
		LVLData.GainedXP = XPTotal;
		XPDataArray.Add(LVLData);

		PartyMembersAlive[i]->BattleStatsComponent->GainXP(XPTotal);
	}
	//TODO: Add Job XP

}

//Sets the boolean to true/false
void ABattleManager::SetBattleNav(bool boolToSet)
{
	CanBattleNav = boolToSet;
}

//Used to fill the turn order array and sort the portraits based on Agility, since fastest character should move first
void ABattleManager::DecideTurnOrder()
{
	//If the Turn Order is empty, fill it with the alive members
	if (TurnOrder.IsEmpty()) {
		TurnOrder.Append(PartyMembersAlive);
		TurnOrder.Append(EnemyMembersAlive);

		//Sort it by Agility
		TurnOrder.Sort(ABattleManager::SortPredicate);
		if (DEController->CreatedBattleWidget) {
			DEController->CreatedBattleWidget->ReplaceTurnOrder(TurnOrder);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Turn Order Succeeded")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Turn Order Failed")));
		}
	}
	
	UpdateTurnOrder();
}

//Used to update portrait order and charge the CTB value
void ABattleManager::UpdateTurnOrder()
{
	//Call recursive function until CTB threshold is met
	SelectActiveCharacter();

	StartTurn_Implementation();


}

//Sort by Agility
bool ABattleManager::SortPredicate(const ABattleChar_BASE& itemA, const ABattleChar_BASE& itemB)
{
	return (itemA.BattleStatsComponent->InitialAttributes.Agility >= itemB.BattleStatsComponent->InitialAttributes.Agility);
}

//Sort by CTB Value
bool ABattleManager::CTBSortPredicate(const ABattleChar_BASE& itemA, const ABattleChar_BASE& itemB)
{
	return (itemA.CTBValue >= itemB.CTBValue);
}

//Called to iterate on TurnOrder and increase CTB Value
void ABattleManager::SelectActiveCharacter()
{
	//TurnOrder.Sort(ABattleManager::CTBSortPredicate);

	//Increase CTB value for each battle character in the turn order
	TurnOrder.Sort(CTBSortPredicate);
	DEController->CreatedBattleWidget->SortTurnPortraits(TurnOrder);
	for (int i = 0; i < TurnOrder.Num(); i++) {
		//If threshold is met, break out of loop
		if (TurnOrder[i]->ChargeTurn()) {
			ActiveCharacter = TurnOrder[i];
			break;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(TurnOrder[i]->CTBValue) + " " +TurnOrder[i]->GetName());
	}

	//If threshold is met and active character is set
	if (ActiveCharacter) {
		//Reset Defending Status
		ActiveCharacter->SetDefending(false);
		TurnOrder.Sort(CTBSortPredicate);
		DEController->CreatedBattleWidget->SortTurnPortraits(TurnOrder);
	}
	else {
		//Loop again
		SelectActiveCharacter();
		
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(ActiveCharacter->CTBValue) + " " + ActiveCharacter->GetName());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, ActiveCharacter->GetName());
	
}

void ABattleManager::HighlightTeam()
{
	TargetCharacters.Empty();
	for (int i = 0; i < EnemyMembersAlive.Num(); i++)
	{
		EnemyMembersAlive[i]->SetPointerVisibilty(EnemyTeamSelected);
		if (EnemyTeamSelected) {
			TargetCharacters.Add(EnemyMembersAlive[i]);
		}
	}
	for (int i = 0; i < PartyMembersAlive.Num(); i++)
	{
		PartyMembersAlive[i]->SetPointerVisibilty(!EnemyTeamSelected);
		if (!EnemyTeamSelected) {
			TargetCharacters.Add(PartyMembersAlive[i]);
		}
	}
}

void ABattleManager::SetUpCamera()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a Battle camera
	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));
	BattleCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	BattleCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	BattleCamera->FieldOfView = 90;
}

void ABattleManager::SetUpTimelines()
{
	MoveToTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
	//Timeline
	MoveToProgressUpdate.BindUFunction(this, FName("MoveToActivePositionUpdate"));
	MoveToFinishedEvent.BindUFunction(this, FName("MoveToActivePositionFinished"));
	MoveToTimelineComponent->SetTimelineFinishedFunc(MoveToFinishedEvent);
	if (GlobalVariables) {
		MoveToTimelineComponent->SetPlayRate(GlobalVariables->PlayRateMoveToActive);
	}
	

	//Create Timeline Component
	MoveFromTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component 2"));
	//Timeline
	MoveFromProgressUpdate.BindUFunction(this, FName("MoveFromActivePositionUpdate"));
	MoveFromFinishedEvent.BindUFunction(this, FName("MoveFromActivePositionFinished"));
	MoveFromTimelineComponent->SetTimelineFinishedFunc(MoveFromFinishedEvent);
	if (GlobalVariables) {
		MoveFromTimelineComponent->SetPlayRate(GlobalVariables->PlayRateMoveFromActive);
	}

	//Create Timeline Component
	AnimChromaticComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component 3"));
	//Timeline
	AnimChromaticProgressUpdate.BindUFunction(this, FName("AnimChromaticUpdate"));
	AnimChromaticFinishedEvent.BindUFunction(this, FName("AnimChromaticFinished"));
	AnimChromaticComponent->SetTimelineFinishedFunc(AnimChromaticFinishedEvent);
	if (GlobalVariables) {
		MoveFromTimelineComponent->SetPlayRate(GlobalVariables->PlayRateAnimChromatic);
	}
	
}

void ABattleManager::SetUpIMC()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(DEController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(inputBattleContext, 0);
	}
}

//Spawns Each Actor for the Enemy and Party Slots
void ABattleManager::SpawnActors()
{
	UWorld* World = GetWorld();
	if (World) {
		FVector Location = this->GetActorLocation();
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		UE_LOGFMT(LogDEPartySubsystemInfo, Display, "Party Member Count: {PartyMemberCount}.", GameInstance->GetPartyMemberSubsystem()->PartyMemberData.Num());
		for (int i = 0; i < PartyMemberLocations.Num(); i++)
		{
			//Check bounds to make sure you can't go beyond the array index
			if (GameInstance->GetPartyMemberSubsystem()->HasValidIndex(i)) {
				FVector NewLocation = Location + PartyMemberLocations[i] + ZAxisOffset;
				if (!GameInstance->GetPartyMemberSubsystem()->PartyMemberData.IsEmpty()) {
					GetWorld()->SpawnActor<ABattleChar_BASE>(GameInstance->GetPartyMemberSubsystem()->GetPartyMemberFromSet(i).BattleCharacterBlueprint, NewLocation, PartyRotation, SpawnInfo);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Party Member Spawned!")));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Party Set!")));
				}
			}
			


		}
		//Grab the enemies from the encounter
		BPEnemyMembersToSpawn = DecideEnemiesToSpawn();
		for (int i = 0; i < EnemyMemberLocations.Num(); i++)
		{
			//check bounds to make sure you do not go beyond array index
			if (BPEnemyMembersToSpawn.IsValidIndex(i)) {
				FVector NewLocation = Location + EnemyMemberLocations[i] + ZAxisOffset;
				if (BPEnemyMembersToSpawn.Num() > 0) {
					GetWorld()->SpawnActor<ABattleChar_BASE>(BPEnemyMembersToSpawn[i], NewLocation, EnemyRotation, SpawnInfo);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Enemy Actor Spawned!")));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Party Set!")));
				}
			}
			


		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Get World Failed!")));
	}




}

//Using the encounter data, decides enemies to spawn into battle
TArray<TSubclassOf<class ABattleCharEnemy_BASE>> ABattleManager::DecideEnemiesToSpawn()
{
	TArray<TSubclassOf<class ABattleCharEnemy_BASE>> ArrayToReturn;
	if (GameInstance->GetPartyMemberSubsystem()->PlayerFieldPosition.AreaInfo) {
		float randomFloat = FMath::RandRange(0.0f, 100.0f);
		float addedProbability = 0.0f;
		TArray<FEncounterData> PossibleEncounters = GameInstance->GetPartyMemberSubsystem()->PlayerFieldPosition.AreaInfo->Encounters;
		for (int i = 0; i < PossibleEncounters.Num(); i++)
		{
			if (!PossibleEncounters[i].CanFlee) {
				DEController->CreatedBattleWidget->ActionMenu->DisableFleeButton();
			}
			addedProbability += PossibleEncounters[i].EncounterProbabilty;
			if (randomFloat <= addedProbability) {
				ArrayToReturn.Append(PossibleEncounters[i].EnemyTypes);
				break;
			}
		}
	}
	else {
		printFStringColor("Area Info Failed", Red));
	}
	if (ArrayToReturn.IsEmpty()) {
		ArrayToReturn.Add(GlobalVariables->DefaultEnemy);
	}
	return ArrayToReturn;

}

void ABattleManager::AddItemToArray(FInventorySlot ItemToAdd)
{
	if (ItemsForWidget.Contains(ItemToAdd))
	{
		ItemsForWidget.Find(ItemToAdd)->Quantity += 1;
		if (ItemsForWidget.Find(ItemToAdd)->Quantity > ItemToAdd.TableRow.GetRow<FBaseItem>("")->MaxStackSize)
		{
			ItemsForWidget.Find(ItemToAdd)->Quantity = ItemToAdd.TableRow.GetRow<FBaseItem>("")->MaxStackSize;
		}
	}
	else {
		ItemsForWidget.Add(ItemToAdd);
	}
}

//Fills the PartyMembers and EnemyMembers arrays 
void ABattleManager::GetBattleMembers()
{
	//Create Array to Find Objects we will need for turns for Party Members
	TArray<AActor*> FoundParty;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleCharParty_Base::StaticClass(), FoundParty);

	//Fill Party Array with Correct Objects
	for (int i = 0; i < FoundParty.Num(); i++) {
		PartyMembers.AddUnique(Cast<ABattleCharParty_Base>(FoundParty[i]));
		PartyMembers[i]->SetUpFromData(GameInstance->GetPartyMemberSubsystem()->GetPartyMemberFromSet(i));
		//Set Battle Manager to this one
		PartyMembers[i]->BattleManager = this;
		//Set the values to the up to date values aka Don't reset to max values
		PartyMembers[i]->BattleStatsComponent->OverrideStateFromSaved(GameInstance->GetPartyMemberSubsystem()->GetPartyMemberFromSet(i).CharacterState);
		//Give the party member their CTB portrait
		PartyMembers[i]->AssignInfoWidget(DEController->CreatedBattleWidget->GetCharacterWidget(i));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Party Member Added!")));
	}

	//Create Array to Find Objects we will need for turns for Enemy Members
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleCharEnemy_BASE::StaticClass(), FoundEnemies);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::FromInt(FoundEnemies.Num()));
	//Fill Enemy Array with Correct Objects
	for (int i = 0; i < FoundEnemies.Num(); i++) {
		EnemyMembers.AddUnique(Cast<ABattleCharEnemy_BASE>(FoundEnemies[i]));
		//Set Battle Manager to this
		EnemyMembers[i]->BattleManager = this;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Enemy Member Added!")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::FromInt(EnemyMembers.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::FromInt(PartyMembers.Num()));
	//Check for any previously KO'd party members and not add them into the array
	for (int i = 0; i < PartyMembers.Num(); i++) {
		if (!PartyMembers[i]->BattleStatsComponent->State.CheckIsKnockedOut()) {
			PartyMembersAlive.Add(PartyMembers[i]);
		}
	}
	EnemyMembersAlive = EnemyMembers;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::FromInt(EnemyMembersAlive.Num()));
}

void ABattleManager::Accept(const FInputActionValue& Value)
{
	if (CanEndBattle) {
		EndBattle_Implementation();
	}
	else {
		if (CanBattleNav) {
			//ActiveCharacter->PlayAttackAnim_Implementation(CurrentSkillData);
			ActiveCharacter->PlayAttackAnim_Implementation(CurrentSkillStruct);
			EndTargetPickMode();
			if (CurrentSkillStruct.isItemSkill) {
				DEController->CreatedBattleWidget->ActionMenu->ItemMenu->UsedItem();
			}
		}
	}

	
}

void ABattleManager::Back(const FInputActionValue& Value)
{
	if (CanBattleNav) {
		EndTargetPickMode();
		DEController->CreatedBattleWidget->SetActionMenuVisible(true);
	}
	
}

void ABattleManager::Left(const FInputActionValue& Value)
{
	if (CanBattleNav) {
		MoveTargetPointer(false);
	}

}

void ABattleManager::Right(const FInputActionValue& Value)
{
	if (CanBattleNav) {
		MoveTargetPointer(true);
	}

}

void ABattleManager::PauseGame(const FInputActionValue& Value)
{
	DEController->PauseGameInCombat();
}


void ABattleManager::MoveToActivePositionUpdate(float alpha)
{
	FVector RaisedActiveLocation = ActiveLocation->GetComponentLocation() + ZAxisOffset;
	Cast<ABattleCharParty_Base>(ActiveCharacter)->PlayApproachAnimation(alpha);
	ActiveCharacter->SetActorLocation(FMath::Lerp(ActiveCharacter->SpawnedLocation, RaisedActiveLocation, alpha), false, nullptr, ETeleportType::None);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Moving Character!")));
}

void ABattleManager::MoveToActivePositionFinished()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Timeline Ended!")));
	DEController->CreatedBattleWidget->SetActionMenuVisible(true);
	//DELAY(2.0, {
	//	MoveFromTimelineComponent->PlayFromStart();
	//	})
}

void ABattleManager::MoveFromActivePositionUpdate(float alpha)
{
	DEController->CreatedBattleWidget->SetActionMenuVisible(false);
	FVector RaisedActiveLocation = ActiveLocation->GetComponentLocation() + ZAxisOffset;
	Cast<ABattleCharParty_Base>(ActiveCharacter)->PlayReturnAnimation(alpha);
	ActiveCharacter->SetActorLocation(FMath::Lerp(RaisedActiveLocation, ActiveCharacter->SpawnedLocation, alpha), false, nullptr, ETeleportType::None);
}

void ABattleManager::MoveFromActivePositionFinished()
{
	DELAY(1.0, {
		ConcludeTurn_Implementation();
		})
}

void ABattleManager::AnimChromaticUpdate(float alpha)
{
	//printFStringColor("Starting Chromatic", Yellow));
	BattleCamera->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(0, 7.5, alpha);
}

void ABattleManager::AnimChromaticFinished()
{

}

void ABattleManager::StartAnimChromatic()
{
	AnimChromaticComponent->PlayFromStart();
}

//Play Approach animation
void ABattleManager::StartMoveToActivePosition()
{
	
	//MoveToActivePositionTimeline.PlayFromStart();
	MoveToTimelineComponent->PlayFromStart();
}

void ABattleManager::StopMoveToActivePosition()
{
	//MoveToActivePositionTimeline.Reverse();
}

//Play Return animation
void ABattleManager::StartMoveToStartingPosition()
{
	
	MoveFromTimelineComponent->PlayFromStart();
}
