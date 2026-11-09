// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Structs/SkillStructs.h>
#include "SkillsMenuWidget.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class DAWNS_END_API USkillsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* SkillMenuActionOverlay;

	UPROPERTY(meta = (BindWidget))
	class UBorder* SkillMenuBorder;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ButtonVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_1;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_2;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_3;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_4;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_5;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SkillButton_6;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> CustomButtonReference;

	UPROPERTY(BlueprintReadOnly)
	class UCustomButtonWidget* CreatedButtonWidget;

	UPROPERTY()
	class ABattleManager* BattleManager;

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_1(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_2(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_3(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_4(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_5(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void SkillButtonClicked_6(UCustomButtonWidget* Button);

	UFUNCTION(BlueprintCallable)
	void UpdateButtons(TArray<class USkillData_PDA*> Skills);

	void UpdateButtons(TSet<FSkillField> Skills);

private:

	UPROPERTY()
	TArray<class UCustomButtonWidget*> SkillButtonArray;

};
