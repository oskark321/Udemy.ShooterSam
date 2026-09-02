// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterSamGameMode.generated.h"

class AShooterAI;
/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AShooterSamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AShooterSamGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//S5 L178
	AShooterAI* ShooterAI;

	TArray<AActor*> EnemyAIArray;
	//tablica która przechwuje dane wskaźnik na aktora

	//S5 L179
	int32 EnemyCount = 0;
};



