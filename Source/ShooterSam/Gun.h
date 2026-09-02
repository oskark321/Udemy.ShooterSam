// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTERSAM_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//S5 L161
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	void PullTrigger();

	//S5 L164
	AController* OwnerController;

	//S5 L165
	UPROPERTY(EditAnywhere)
	float MaxRange = 6000.0f;

	FHitResult HitResult;

	//S5 L166
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitParticles;
	//zmienna gdzie w edytorze dodamy NiagaraSystem

	//UPROPERTY(EditAnywhere)
	//UNiagaraSystem* MuzzleParticles;
	//zmienna gdzie w edytorze dodamy NiagaraSystem

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* MuzzleParticlesComp;
	//komponent widoczny w planie BP naszego aktora czyli broni

	//S5 L167
	UPROPERTY(EditAnywhere)
	float BulletDamage = 10.0f;

	//S5 L191
	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;
	//zmienna gdzie w edytorze dodamy odpowiedni plan dzwięku

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	//zmienna gdzie w edytorze dodamy odpowiedni plan dzwięku
};
