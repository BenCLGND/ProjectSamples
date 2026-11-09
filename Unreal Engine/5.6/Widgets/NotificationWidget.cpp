// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NotificationWidget.h"
#include "Components/TextBlock.h"

void UNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UNotificationWidget::SetMessageText(FText TextToSet)
{
	NotificationText->SetText(TextToSet);
}


