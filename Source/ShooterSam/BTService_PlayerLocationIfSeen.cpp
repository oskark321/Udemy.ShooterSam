// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterSamCharacter.h"
//#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	//S5 L185
	NodeName = TEXT("Update PlayerLocation If Seen");
	//nadaje nazwe nodowi jaka widzimy używamy w BT
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//funkcja wywoływana w koło dopuki usługa będzie aktywna(komponent właściciela - faktyczny komponent BT właściciela usług, pamięć węzła - wskaźnik pamięci, oraz delta Time)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//S5 L186
	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	//z komponentu właściciela pobieramy AI Controller ale naszym AI Controller jest klasa AShooterAI dlatego to rzutowanie
	AShooterSamCharacter* Player = OwnerController->PlayerCharacter;
	//pobieramy wskażnik postaci który mamy juz zapisany w klasie AShooterAI i tam też jest ustawiony wskaźnik postaci
	UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();
	//urzywamy funkcji pobierz komponent tablicy po przez to że OwnerController jest klasy AShooterAI

	if (OwnerController && Player && Blackboard)
	{
		if (OwnerController->LineOfSightTo(Player))
			//sprawdzamy czy AI ma linie zwroku czy widzi gracza Player ale musimy wyciągnąć z gracza AActor - co czynimy wyzej
		{
			OwnerController->SetFocus(Player);
			//jeśli jest widoczny ustawiuamy focus na gracza, że ma skupić się na graczu
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
			//dalej jeśli widzimy gracza ustawiamy lokalizację gracza aby tam poszedł (ustawiamy nazwę klucza gdzie ma dać dane - ta nazwa klucza pobierana jest z klucza jaki ustawiliśmy 
			//wewnątrz usługi jako główny klucz którym ma się kierować ten Node ten BTService w BT w edytorze tu PlayerLocation)

		}
		else
		{
			OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
			//usuwa fokus z gracza aby go nie śledził AI (najwyższy priorytet w SetFocus Gameplay jest ustawiony domyślnie)
			Blackboard->ClearValue(GetSelectedBlackboardKey());
			//czyścimy wartość zmienną (ustawiamy nazwę klucza ktore dane ma wyczyścić - ta nazwa klucza pobierana jest z klucza jaki ustawiliśmy wewnątrz usługi jako główny klucz którym ma
			//się kierować ten Node ten BTService w BT w edytorze tu PlayerLocation)
		}
	}

}
