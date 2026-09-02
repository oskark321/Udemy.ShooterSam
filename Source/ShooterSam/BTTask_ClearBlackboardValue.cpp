// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	//S5 L188
	NodeName = TEXT("Clear Blackboard Value");
}

//S5 L188
EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//ta funkcja rozpocznie zadanie i zwarca wynik :pomyślny, nieudany lub w toku (komponent właściciela - faktyczny komponent BT właściciela usług, pamięć węzła - wskaźnik pamięci)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//S5 L188
	//pobierz komponent tablicy
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	//urzywamy funkcji pobierz komponent tablicy po przez komponent właściciela tej usługi
	
	//wyczyścić wartość wybranego klucza
	if (Blackboard)
	{
		Blackboard->ClearValue(GetSelectedBlackboardKey());
		//czyścimy wartość zmienną (ustawiamy nazwę klucza ktore dane ma wyczyścić - ta nazwa klucza pobierana jest z klucza jaki ustawiliśmy wewnątrz usługi jako główny klucz którym ma
		//się kierować ten Node ten BTService w BT w edytorze tu LastKnownPlayerLocaton)
	}
	//zwrocic sukces lub porazke - tu zawsze bedzie sukces
	return EBTNodeResult::Succeeded;
}
