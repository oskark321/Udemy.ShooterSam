// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAI.generated.h"

class AShooterSamCharacter;

/**
 * 
 */
UCLASS()
class SHOOTERSAM_API AShooterAI : public AAIController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//S5 L174
	APawn* PlayerPawn;

	//S5 L175
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200.0f;

	//S5 L176
	bool IsVisible = false;

	//S5 L177
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyAIBehaviorTree;
	//zmienna gdzie w edytorze dodajemy odpowiednie drzewko zachowań

	//S5 L178
	AShooterSamCharacter* PlayerCharacter;
	AShooterSamCharacter* MyCharacter;

	void StartBehaviorTree(AShooterSamCharacter* Player);

};
