// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/ProgressBar.h"

void UHUDWidget::SetHealthBarPercent(float NewPercent)
{
	//S5 L190
	if (NewPercent >= 0.0f && NewPercent <= 1.0f)
	//działamy tylko jeśli zdrowie mamy miedzy 0 - czyli śmierć a 1 - czyli full zdrowie 0 a 1 w tym przedziale procentowym
	{
		HealthBar->SetPercent(NewPercent);
		//wywołujemy pasek zdrowia i wgudowaną w PrograssBar funkcję SetPercent - ustaw procent
	}
}
