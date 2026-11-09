// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Combat/Party/BattleCharParty_Base.h"
#include "Combat/Enemy/BattleCharEnemy_BASE.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "System/GlobalVariables_PDA.h"
#include "BattleManager.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FLevelDisplayData 
{

	GENERATED_BODY()

	UPROPERTY()
	FText CharacterName;

	UPROPERTY()
	int StartXP;

	UPROPERTY()
	int StartLevel;

	UPROPERTY()
	int GainedXP; 

};

UCLASS()
class DAWNS_END_API ABattleManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABattleManager();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	

	/*	Components	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//Battle Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* BattleCamera;

	//Location to move active character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Misc, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActiveLocation;

	//Timeline Component for Movement to Active Position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Misc, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* MoveToTimelineComponent;

	//Timeline Component for Movement from Active Position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Misc, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* MoveFromTimelineComponent;

	//Timeline Component for Chromatic Aberration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Misc, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* AnimChromaticComponent;

	/*	Functions	*/
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Custom Functions//

	
	UFUNCTION(BlueprintNativeEvent)
	void BeginBattle();

	void BeginBattle_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void StartTurn();

	void StartTurn_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void ConcludeTurn();

	void ConcludeTurn_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void EndBattle();

	void EndBattle_Implementation();

	UFUNCTION(BlueprintCallable)
	void StartTargetPickMode(int SkillIndex);
	void StartTargetPickMode(FSkillField SkillTableData);

	UFUNCTION(BlueprintCallable)
	void EndTargetPickMode();

	UFUNCTION(BlueprintCallable)
	void MoveTargetPointer(bool MoveRight);

	UFUNCTION(BlueprintCallable)
	void ApplyBattleEffect(UBattleStatsComponent* InstigatorStats);

	UFUNCTION(BlueprintCallable)
	void EnemyTurn();

	UFUNCTION(BlueprintCallable)
	void DecideTargetPlayer();

	UFUNCTION()
	void EnemyFinishedAttack();

	UFUNCTION(BlueprintCallable)
	void StartDefending();

	UFUNCTION(BlueprintCallable)
	void UpdateAliveMembers(ABattleChar_BASE* CharacterToAdjust, bool isDead);

	UFUNCTION(BlueprintCallable)
	void RemoveFromTurnOrder(ABattleChar_BASE* CharacterToRemove);

	UFUNCTION(BlueprintCallable)
	void AddToTurnOrder(ABattleChar_BASE* CharacterToAdd);

	UFUNCTION(BlueprintCallable)
	void BattleResult(EEnumBattleOutcome BattleOutcome);

	UFUNCTION(BlueprintCallable)
	void AttemptFlee();

	UFUNCTION()
	void SkipPartyTurn();

	UFUNCTION(BlueprintCallable)
	void UpdateEnemySkill();

	UFUNCTION(BlueprintCallable)
	TArray<ABattleChar_BASE*> GetTargetTeam();

	UFUNCTION(BlueprintCallable)
	void DecideDrops();

	UFUNCTION(BlueprintCallable)
	void YieldPartyXP();

	UFUNCTION()
	void SetBattleNav(bool boolToSet);


	/*	Variables	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Variables", meta = (AllowPrivateAccess = "true"), 
		meta = (ToolTip = "Edit these to decide where the players spawn into the battle."))
	TArray<FVector> PartyMemberLocations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Variables", meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Edit these to decide where the enemies spawn into the battle."))
	TArray<FVector> EnemyMemberLocations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Variables", meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Used to ensure the party spawns facing the correct direction."))
	FRotator PartyRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Variables", meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Used to ensure no sprites are floating."))
	FVector ZAxisOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Variables", meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Used to ensure the enemy spawns facing the correct direction."))
	FRotator EnemyRotation;

	UPROPERTY()
	TArray<TSubclassOf<class ABattleCharEnemy_BASE>> BPEnemyMembersToSpawn;

	UPROPERTY(BlueprintReadOnly)
	TArray<UClass*> PartyMembersToSpawn;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharEnemy_BASE*> EnemyMembers;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharParty_Base*> PartyMembers;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharEnemy_BASE*> EnemyMembersAlive;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharParty_Base*> PartyMembersAlive;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharParty_Base*> PartyMembersDead;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharEnemy_BASE*> EnemyMembersDead;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABattleCharParty_Base*> ReservePartyMembers;

	UPROPERTY(BlueprintReadOnly)
	TArray<ABattleChar_BASE*> TurnOrder;

	UPROPERTY(BlueprintReadOnly)
	ABattleChar_BASE* ActiveCharacter;

	UPROPERTY(BlueprintReadOnly)
	TArray<ABattleChar_BASE*> TargetCharacters;

	UPROPERTY(BlueprintReadOnly)
	int TargetIndex;

	UPROPERTY(EditAnywhere, Category = "Timeline Settings",
		meta = (ToolTip = "Used to control how the active character moves into their turn position. \n This must be set to ensure no errors."))
	UCurveFloat* MoveToActivePositionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Timeline Settings", 
		meta = (ToolTip = "Used to control how the chromatic aberration of the camera works. \n This must be set to ensure no errors."))
	UCurveFloat* AnimChromaticCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* inputBattleContext;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* AcceptAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* BackAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* LeftAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* RightAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* PauseAction;

	UPROPERTY()
	class UGlobalVariables_PDA* GlobalVariables;

	void StartMoveToActivePosition();
	void StopMoveToActivePosition();

	UFUNCTION(BlueprintCallable)
	void StartMoveToStartingPosition();

	FORCEINLINE class ADawnsEndPlayerController* GetDEController() const { return DEController; }

	FORCEINLINE bool GetCanEndBattle() const { return CanEndBattle; }

private:

	UPROPERTY()
	bool EnemyTeamSelected = true;

	UPROPERTY()
	TSet<FInventorySlot> ItemsForWidget;

	UPROPERTY()
	int AurumToAdd = 0;

	UPROPERTY()
	int XPTotal = 0;

	UPROPERTY()
	int JPTotal = 0;

	UPROPERTY()
	bool CanBattleNav;

	UPROPERTY()
	bool CanEndBattle;

	UPROPERTY()
	bool LostBattle = false;

	UPROPERTY()
	FSkillField CurrentSkillStruct;

	UPROPERTY()
	class UDawnsEndGameInstance* GameInstance;

	UPROPERTY()
	class ADawnsEndPlayerController* DEController;

	UPROPERTY()
	TArray<FLevelDisplayData> XPDataArray;

	/*	Timeline Variables	*/	
	FOnTimelineEvent MoveToFinishedEvent;
	FOnTimelineFloat MoveToProgressUpdate;
	FOnTimelineEvent MoveFromFinishedEvent;
	FOnTimelineFloat MoveFromProgressUpdate;

	UFUNCTION()
	void MoveToActivePositionUpdate(float alpha);

	UFUNCTION()
	void MoveToActivePositionFinished();

	UFUNCTION()
	void MoveFromActivePositionUpdate(float alpha);

	UFUNCTION()
	void MoveFromActivePositionFinished();

	FOnTimelineEvent AnimChromaticFinishedEvent;
	FOnTimelineFloat AnimChromaticProgressUpdate;

	UFUNCTION()
	void AnimChromaticUpdate(float alpha);

	UFUNCTION()
	void AnimChromaticFinished();

	void StartAnimChromatic();

	//Util Functions
	void SpawnActors();

	TArray<TSubclassOf<class ABattleCharEnemy_BASE>> DecideEnemiesToSpawn();

	void AddItemToArray(FInventorySlot ItemToAdd);

	void GetBattleMembers();

	void DecideTurnOrder();

	void UpdateTurnOrder();

	static bool SortPredicate(const ABattleChar_BASE& itemA, const ABattleChar_BASE& itemB);

	static bool CTBSortPredicate(const ABattleChar_BASE& itemA, const ABattleChar_BASE& itemB);

	void SelectActiveCharacter();

	void HighlightTeam();

	void SetUpCamera();

	void SetUpTimelines();

	void SetUpIMC();

	void Accept(const FInputActionValue& Value);

	void Back(const FInputActionValue& Value);

	void Left(const FInputActionValue& Value);

	void Right(const FInputActionValue& Value);

	void PauseGame(const FInputActionValue& Value);
};
