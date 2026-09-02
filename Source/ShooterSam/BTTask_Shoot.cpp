// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "ShooterAI.h"
#include "ShooterSamCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot At Player");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//ta funkcja rozpocznie zadanie i zwarca wynik :pomyślny, nieudany lub w toku (komponent właściciela - faktyczny komponent BT właściciela usług, pamięć węzła - wskaźnik pamięci)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//S5 L189
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	//typ zmiennej oraz nazwa rezultat aby zapisac razultat ktory ma zwrocic funkcja - ustawiony na niepowodzenie bo jesli
	//nie wejdzie do IF to funkcja skoczy się niepowodzeniem bo w if mamy sukces

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	//z komponentu właściciela pobieramy AI Controller ale naszym AI Controller jest klasa AShooterAI dlatego to rzutowanie

	if (OwnerController)
	{
		AShooterSamCharacter* OwnerCharacter = OwnerController->MyCharacter;
		//pobieramy postać którą kontroluje AIController czyli przeciwnika bota
		AShooterSamCharacter* PlayerCharacter = OwnerController->PlayerCharacter;
		//pobieramy postać grającą po przez kontroler AI ponieważ w klasie ShooterAI mamy tam daną naszą postać pobraną zapisaną
		 
		if (OwnerCharacter && PlayerCharacter && PlayerCharacter->IsAlive)
		//w klasie ShooterSamCharacter mamy zmienną IsAlive - więc strzelamy do wroga tylko jeśli żyje
		{
			OwnerCharacter->Shoot();
			//jeśli if jest prawdą czyli postać gracza żyje i jest w zasięgu - ustawiamy to w Node MoveTo w BT, to niech postać AI strzela
			Result = EBTNodeResult::Succeeded;
			//gdy gracz żyje i jest w zasięgu widzenia co sprawdzamy w klasie BTService_PlayerLocationIfSeen i jest w zasięgu - ustawiamy to w Node MoveTo w BT to strzelamy i ustawiamy
			//Result - Rezultat na sukces by na końcu funkcji zwrócioć tylko rezultat i jeśli wejdzie w if to będzie to sukces
			
		}
	}

	return Result;
	//tutaj zwracamy Rezult - Rezultat jeśli nie wejdzie w if to bedzie fauszywy więc AI nie strzeliło do gracza a Node Sequence w BT też będzie fauszywy i skończy powtarzanie
	//jeśli wejdzie do if czyli AI strzeliło do Gracza to zwróci sukces więc Node Sequence w BT się powtórzy
}
