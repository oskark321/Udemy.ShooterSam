// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAI.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterSamCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();

	//S5 L174
	//PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//pobieramy pionek gracza czyli wskazujemy na postać którą gramy my

}

void AShooterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	//S5 L175
	if (PlayerPawn)
	{
		//S5 L176
		IsVisible = LineOfSightTo(PlayerPawn);
		//jesli aktor - tutaj pionek gracza - jest widoczny zwraca nam true
		if (IsVisible)
		{
			//S5 L174
			SetFocus(PlayerPawn);
			//ustawiuamy focus na gracza, że ma skupić się na graczu

			//S5 L175
			MoveToActor(PlayerPawn, AcceptanceRadius);
			//podążaj za aktorem(tutaj za jakim aktorem my mamy pionek gracza, jak blisko ma się zbliżyć tutaj 200)
		}
		else
		{
			//S5 L176
			ClearFocus(EAIFocusPriority::Gameplay);
			//usuwa fokus z gracza aby go nie śledził AI (najwyższy priorytet w SetFocus Gameplay jest ustawiony domyślnie)

			//StopMovement();
			//tutaj pauzujemy ruch ale ja tego nie chce

		}

	}
	*/


}

void AShooterAI::StartBehaviorTree(AShooterSamCharacter* Player)
//funkcja SBT jest wywołana w GameMode
{
	//S5 L177
	if (EnemyAIBehaviorTree)
	{
		//S5 L178
		MyCharacter = Cast<AShooterSamCharacter>(GetPawn());
		//tutaj mamy postać którą kontroluje AIController

		if (Player)
		{
			PlayerCharacter = Player;
			//tutaj postać gracza
		}
		//S5 L177
		RunBehaviorTree(EnemyAIBehaviorTree);
		//uruchamia drzewo zachowań (zmienna gdzie w edytorze dodajemy odpowiednie drzewko zachowań)

		//S5 L180
		UBlackboardComponent* MyBlackboard = GetBlackboardComponent();
		//pobieramy tablicę i zapisujemy do zmiennej

		if (MyBlackboard && PlayerCharacter && MyCharacter)
			{
				//MyBlackboard->SetValueAsVector("PlayerLocation", PlayerCharacter->GetActorLocation());
				//ustawia w tablicy wartosc jako wektor(nazwa klucza/zmiennej w tablicy która chcemy ustawić, no i jaka wartość)

				MyBlackboard->SetValueAsVector("StartEnemyLocation", MyCharacter->GetActorLocation());
				//tutaj ustawiamy pozycje startową wrogów
			}
	}

}

