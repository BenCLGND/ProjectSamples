// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Player/DawnsEndCharacter.h"
#include "SUDS/Public/SUDSParticipant.h"
#include "System/DE_Enums.h"
#include "Interactables/InteractableInterface.h"
#include "BaseNPC.generated.h"

class AAIC_NPC;
class USplineComponent;
class USUDSScript;
class ADawnsEndCharacter;
class USphereComponent;
/**
 * 
 */
UCLASS()
class DAWNS_END_API ABaseNPC : public ADawnsEndCharacter, public ISUDSParticipant, public IInteractableInterface
{
	GENERATED_BODY()



	/** Collision Sphere */
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCol;

public:
	virtual void BeginPlay() override;

	virtual void OnDialogueStarting_Implementation(USUDSDialogue* Dialogue, FName AtLabel) override;

	virtual void OnDialogueEvent_Implementation(USUDSDialogue* Dialogue, FName EventName, const TArray<FSUDSValue>& Arguments) override;

	virtual void OnDialogueFinished_Implementation(USUDSDialogue* Dialogue) override;


	UFUNCTION()
	void CreateFromUtility(FText NPCNameToSet, USUDSScript* SUDSScriptToSet, TMap<E_NPCSkinLayers, class UPaperZDAnimationSkin*> NPCSkinMapToSet, class UPaperFlipbook* FlipbookToSet);

	UFUNCTION()
	void SetDialogue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UnSetDialog(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//Call from dialogue to run this [event CompleteTaskEvent `QuestIdentifier` `TaskIdentifier`]
	UFUNCTION()
	void CompleteTaskEvent(FName QuestID, FName TaskIdentifier);

	//Call from dialogue to run this [event GiveItemEvent]
	UFUNCTION()
	void GiveItemEvent();

	UFUNCTION(BlueprintCallable)
	virtual void PlayerInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsInteractable() override;

	UFUNCTION(BlueprintCallable)
	virtual void CheckIfObjectTriggered() override;

	UFUNCTION(BlueprintCallable)
	virtual void AddTriggeredObject() override;

	ABaseNPC();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Data")
	FText NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Data")
	USUDSScript* MySUDSScript;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Data")
	bool isUnique = false;
	
	UPROPERTY()
	class ADawnsEndPlayerCharacter* Player;

	UPROPERTY(EditAnywhere, Category = "NPC")
	TMap<E_NPCSkinLayers, class UPaperZDAnimationSkin*> NPCSkinMap;


#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif //WITH_EDITOR
	

protected:
	UPROPERTY()
	class UDawnsEndGameInstance* GameInst;

	void SetSkins();

	UPROPERTY(EditAnywhere, Category = "NPC Data")
	FGuid GUID;

	UPROPERTY()
	bool cannotSpawn = false;
};
