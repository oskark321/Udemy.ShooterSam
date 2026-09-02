// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SHOOTERSAM_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional));
	//BindWidget musimy użyć aby zmienna którą mamy w C++ została powiązana z widgetem wewnętrznym w BP
	UProgressBar* HealthBar;
	//tworzymy komponent ProgrssBar - nazwa musi być taka sama jak nazwa ProgressBar w edytorze, musimy dodać biblioteki

	//S5 L190
	void SetHealthBarPercent(float NewPercent);
};
