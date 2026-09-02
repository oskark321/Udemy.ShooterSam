// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamGameMode.h"
#include "ShooterAI.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterSamCharacter.h"


AShooterSamGameMode::AShooterSamGameMode()
{
	// stub
}

void AShooterSamGameMode::BeginPlay()
{
	Super::BeginPlay();

	//S5 L178
	AShooterSamCharacter* Player = Cast<AShooterSamCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//pobieramy pionek gracza czyli wskazujemy na postać którą gramy my

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), EnemyAIArray);
	//funkcja ta znajduje wszystkich aktorów danej klasy w świecie(jaki świat, podklasa której obiektem jest UClass - reprezentujemy klase której chcemy obiekty liczyć a StaticClass() - jest to
	//funkcja która zwróci obiekty których szukamy, tablica gdzie wypełni sie wskaźnikamy aktorów w sensie ile ich mamy w świecie gry)

	//S5 L179
	EnemyCount = EnemyAIArray.Num();
	//uzyskujemy liczbę ile mamy elementów w tabliczy czyli w tym wypadku ile mamy aktorów klasy AShooterAI czyli w praktyce przeciwników

	/*
	int32 LoopIndex = 0;

	while (LoopIndex < EnemyCount)
	{
		AActor* ShooterAIActor = EnemyAIArray[LoopIndex];
		//element z tablicy zapisujemy do zmiennej lokalnej

		LoopIndex++;
		UE_LOG(LogTemp, Warning, TEXT("Loop Count : %d"), LoopIndex);
	}
	*
	//to samo w petli for
	for (int32 LoopIndex = 0; LoopIndex < EnemyCount; LoopIndex++)
	{
		AActor* ShooterAIActor = EnemyAIArray[LoopIndex];
		//element z tablicy zapisujemy do zmiennej lokalnej
		ShooterAI = Cast<AShooterAI>(ShooterAIActor);
		//pobranie wskaźnika ShooterAIActor i obsadzenie go jako AShooterAI i zapisanie do zmiennej ShooterAI

		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);
			//przekazujemy gracza czyli naszą postać do funkcji SBT w klasie AShooterAI
			UE_LOG(LogTemp, Warning, TEXT("%s starting behavior tree"), *ShooterAI->GetActorNameOrLabel());
		}


		UE_LOG(LogTemp, Warning, TEXT("Loop Count : %d"), LoopIndex);
	}
	*/
	//S5 L181
	for (AActor* ShooterAIActor : EnemyAIArray)
		//pętla for oparta na zasięgu, trochę gorzej i mniej funkcjonalnie co pętla for oparta na indeksie petli tutaj bedzie działąć puki są elementy w tablicy
	{
		ShooterAI = Cast<AShooterAI>(ShooterAIActor);
		//pobranie wskaźnika ShooterAIActor i obsadzenie go jako AShooterAI i zapisanie do zmiennej ShooterAI

		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);
			//przekazujemy gracza czyli naszą postać do funkcji SBT w klasie AShooterAI
			UE_LOG(LogTemp, Warning, TEXT("%s starting behavior tree"), *ShooterAI->GetActorNameOrLabel());
		}


		//UE_LOG(LogTemp, Warning, TEXT("Loop Count : %d"), LoopIndex);
	}
}
