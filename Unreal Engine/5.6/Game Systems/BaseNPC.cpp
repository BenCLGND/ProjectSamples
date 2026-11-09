// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/BaseNPC.h"
#include "System/GUIDHandlerSubsystem.h"
#include "SUDSDialogue.h"
#include "SUDSLibrary.h"
#include "Components/SphereComponent.h"
#include "Player/DawnsEndPlayerCharacter.h"
#include "CustomLogging/CustomLogCategories.h"
#include "Misc/DataValidation.h"
#include "System/DawnsEndGameInstance.h"
#include "System/QuestSubsystem.h"
#include "System/DE_Enums.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperFlipbookComponent.h"

void ABaseNPC::SetDialogue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ADawnsEndPlayerCharacter* Character = Cast<ADawnsEndPlayerCharacter>(OtherActor))
	{
		Player = Character;
		Character->SetSpeakerNPC(this);
		Character->SetActiveSUDSScript(MySUDSScript);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ABaseNPC::UnSetDialog(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ADawnsEndPlayerCharacter* Character = Cast<ADawnsEndPlayerCharacter>(OtherActor))
	{
		Character->SetSpeakerNPC(nullptr);
		Character->SetActiveSUDSScript(nullptr);
	}
	if (isUnique)
	{
		AddTriggeredObject();
		UE_LOGFMT(LogGUIDSubsystem, Display, "Added: {PartyMemberCount}.", this->NPCName.ToString());
	}
}

void ABaseNPC::CompleteTaskEvent(FName QuestID, FName TaskIdentifier)
{
	if(GameInst->GetQuestSubsystem()->QuestProgression->IsTaskRelevant(QuestID, TaskIdentifier))
	{
		GameInst->GetQuestSubsystem()->QuestProgression->CompleteTask(QuestID, TaskIdentifier);
	}
}

void ABaseNPC::GiveItemEvent()
{

}



ABaseNPC::ABaseNPC()
{
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SphereCol->SetupAttachment(RootComponent);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &ABaseNPC::SetDialogue);
	SphereCol->OnComponentEndOverlap.AddDynamic(this, &ABaseNPC::UnSetDialog);
	SphereCol->SetSphereRadius(100.f);


}





void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	GameInst = Cast<UDawnsEndGameInstance>(GetGameInstance());
	if (isUnique) {
		CheckIfObjectTriggered();
	}
	//Set Skins Here
	SetSkins();
}

#if WITH_EDITOR
EDataValidationResult ABaseNPC::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!MySUDSScript)
	{
		Context.AddError(INVTEXT("Please set the dialogue."));
		return EDataValidationResult::Invalid;
	}
	if (NPCSkinMap.IsEmpty()) 
	{
		Context.AddError(INVTEXT("Please set skins for the NPC."));
		return EDataValidationResult::Invalid;
	}
	if (NPCName.ToString() == "")
	{
		Context.AddError(INVTEXT("Please name for the NPC."));
		return EDataValidationResult::Invalid;
	}
	return EDataValidationResult::Valid;
}

#endif //WITH_EDITOR

void ABaseNPC::SetSkins()
{
	if (NPCSkinMap.IsEmpty())
	{
		return;
	}
	if (NPCSkinMap[E_NPCSkinLayers::Hat] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Hat], 9);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Hair] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Hair], 8);
	}
	if (NPCSkinMap[E_NPCSkinLayers::FacialHair] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::FacialHair], 7);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Coat] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Coat], 6);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Top] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Top], 5);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Belt] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Belt], 4);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Accessory] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Accessory], 3);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Bottoms] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Bottoms], 2);
	}
	if (NPCSkinMap[E_NPCSkinLayers::Footwear] != NULL)
	{
		GetAnimationComponent()->GetAnimInstance()->ApplyAnimationSkinToLayer(NPCSkinMap[E_NPCSkinLayers::Footwear], 1);
	}

}

void ABaseNPC::PlayerInteract()
{
	//Move start of dialogue here
}

bool ABaseNPC::IsInteractable()
{
	return !cannotSpawn;
}

void ABaseNPC::CheckIfObjectTriggered()
{
	if (GUID.IsValid())
	{
		if (GameInst->GetGUIDSubsystem()->TriggeredNPCs.Contains(GUID))
		{
			cannotSpawn = true;
			Destroy();
			UE_LOGFMT(LogGUIDSubsystem, Display, "Destroyed: {PartyMemberCount}.", this->NPCName.ToString());
		}
	}
	else {
		UE_LOGFMT(LogGUIDSubsystem, Warning, "Invalid GUID.");
	}
}

void ABaseNPC::AddTriggeredObject()
{
	if (GUID.IsValid())
	{
		GameInst->GetGUIDSubsystem()->TriggeredNPCs.Add(GUID);
	}
	else {
		UE_LOGFMT(LogGUIDSubsystem, Warning, "Invalid GUID.");
	}
}

void ABaseNPC::OnDialogueStarting_Implementation(USUDSDialogue* Dialogue, FName AtLabel)
{
	ISUDSParticipant::OnDialogueStarting_Implementation(Dialogue, AtLabel);

	Dialogue->SetVariableText(FName("SpeakerName.NPC"), NPCName);
}

void ABaseNPC::OnDialogueEvent_Implementation(USUDSDialogue* Dialogue, FName EventName,
	const TArray<FSUDSValue>& Arguments)
{
	ISUDSParticipant::OnDialogueEvent_Implementation(Dialogue, EventName, Arguments);
	//UE_LOGFMT(LogSUDSDialogue, Display, "Event Called: {title}.", EventName.ToString());
	if (EventName == "CompleteTask")
	{
		CompleteTaskEvent(Arguments[0].GetVariableNameValue(), Arguments[1].GetVariableNameValue());
	}
	else if (EventName == "GiveItem")
	{
		GiveItemEvent();
	}
	
}

void ABaseNPC::OnDialogueFinished_Implementation(USUDSDialogue* Dialogue)
{
	if (isUnique)
	{
		AddTriggeredObject();
		UE_LOGFMT(LogGUIDSubsystem, Display, "Added: {PartyMemberCount}.", this->NPCName.ToString());
	}
}

void ABaseNPC::CreateFromUtility(FText NPCNameToSet, USUDSScript* SUDSScriptToSet, TMap<E_NPCSkinLayers, class UPaperZDAnimationSkin*> NPCSkinMapToSet, 
	class UPaperFlipbook* FlipbookToSet)
{
	NPCName = NPCNameToSet;
	MySUDSScript = SUDSScriptToSet;
	NPCSkinMap = NPCSkinMapToSet;
	GetSprite()->SetFlipbook(FlipbookToSet);
}



