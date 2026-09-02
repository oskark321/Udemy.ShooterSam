// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LastKnownPlayerLoc.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterSamCharacter.h"
#include "Kismet\GameplayStatics.h"

UBTService_LastKnownPlayerLoc::UBTService_LastKnownPlayerLoc()
{
	//S5 L187
	NodeName = TEXT("Get Player Location");
	//nadaje nazwe nodowi jaka widzimy używamy w BT
}

void UBTService_LastKnownPlayerLoc::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//funkcja wywoływana w koło dopuki usługa będzie aktywna(komponent właściciela - faktyczny komponent BT właściciela usług, pamięć węzła - wskaźnik pamięci, oraz delta Time)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//S5 L187
	//AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	//z komponentu właściciela pobieramy AI Controller ale naszym AI Controller jest klasa AShooterAI dlatego to rzutowanie
	//AShooterSamCharacter* Player = OwnerController->PlayerCharacter;
	//pobieramy wskażnik postaci który mamy juz zapisany w klasie AShooterAI i tam też jest ustawiony wskaźnik postaci
	//UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();
	//urzywamy funkcji pobierz komponent tablicy po przez to że OwnerController jest klasy AShooterAI

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//pobieramy pionek gracza
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	//urzywamy funkcji pobierz komponent tablicy po przez komponent właściciela tej usługi

	if (PlayerPawn && Blackboard)
	{
		Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
		//ustawiamy ostatnią znaną lokalizację gracza po przez klucz który jest użyty w tym node
	}
}
