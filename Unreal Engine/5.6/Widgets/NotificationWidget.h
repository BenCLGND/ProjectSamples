// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class DAWNS_END_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	class UBorder* NotificationBorder;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotificationText;

	UFUNCTION(BlueprintCallable)
	void SetMessageText(FText TextToSet);
};
