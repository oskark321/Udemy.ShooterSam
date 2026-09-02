// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LastKnownPlayerLoc.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERSAM_API UBTService_LastKnownPlayerLoc : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	//S5 L187
	UBTService_LastKnownPlayerLoc();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//funkcja wywoływana w koło dopuki usługa będzie aktywna(komponent właściciela - faktyczny komponent BT właściciela usług, pamięć węzła - wskaźnik pamięci, oraz delta Time)
};
