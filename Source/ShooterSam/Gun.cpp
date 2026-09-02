// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

/*
//S5 L171
#define PII 3.141592f
#define LOG(x) UE_LOG(LogTemp, Warning, TEXT(x))
#define WLOG(x, y) UE_LOG(LogTemp, Warning, TEXT(x), y)

//S5 L172
class Shape //rodzic
{
protected:
public:
	int32 Sides;

//S5 L173
public:
	virtual void Explain(); //virtual - dzięki temu C++ sprawdza klay podrzędne czy tam też nie ma funkcji Explain()
};

void Shape::Explain()
{
	LOG("I'm a Shape");
}

class Rectangle : public Shape //potomek
{
public:
	void SetSides(int32 Size);

	virtual void Explain() override; //override - opcjonalne ale dzięki temu C++ sprawdza czy w klasie nadrzędnej jest taka funkcja, pomaga przy pisowni aby nie zrobić literówki
	//virtual - ponownie możemy zrobić klasę podrzędną z klasy Rectangle i ponownie możemy tam umieścić taką samą funkcję
};

void Rectangle::SetSides(int32 Size)
{
	Sides = Size;
}

void Rectangle::Explain()
{
	LOG("I'm a Rectangle");
}
*/

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//S5 L161
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//tworzymy obiekt typu USceneComponent który później będzie widoczny w planie Bp w edytorze
	SetRootComponent(SceneComp);
	//ustawiamy nasz kompoinent jako komponent główny

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	//tworzymy obiekt typu USkieletalMeshComponent który później będzie widoczny w planie Bp w edytorze

	SkeletalMeshComp->SetupAttachment(RootComponent);
	//dodajemy SMC do root component

	//S5 L166
	MuzzleParticlesComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleParticlesComp"));
	//tworzymy w planie BP aktora komponent dla NiagaraSystem

	MuzzleParticlesComp->SetupAttachment(SkeletalMeshComp);
	//podpinamy komponent Niagara pod komponent siatki w planie BP
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	//S5 L166
	MuzzleParticlesComp->Deactivate();
	//dezaktywujemy komponent dla bezpieczństwa na początku gry

	/*
	//S5 L171
	UE_LOG(LogTemp, Warning, TEXT("The numer PI is: %f"), PII);
	LOG("Hallo"); //UE_LOG(LogTemp, Warning, TEXT("Hallo"))
	WLOG("Dengery and PI: %f", PII); //UE_LOG(LogTemp, Warning, TEXT("Dengery"))

	//S5 L172
	Rectangle MyRect;
	MyRect.Sides = 2;
	MyRect.SetSides(4);

	Shape MyShape;
	MyShape.Sides = 10;

	//S5 L173
	//MyShape.Explain();
	//MyRect.Explain();

	Shape* ShapePtr1 = &MyShape;
	Shape* ShapePtr2 = &MyRect;
	ShapePtr1->Explain();
	ShapePtr2->Explain();
	*/
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	//S5 L166
	MuzzleParticlesComp->Activate(true);
	//aktywujemy komponent gdy naciśniemy spust (ustawiamy reset na true) dla bezpieczeństwa aby na pewno wyświetlił nam się efelkt który jest przypisany w tym komponęcie

	//S5 L191
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	//funkcja odtwarzająca dzwięk w danej lokalizacji(pobieramy świat że w tym świecie, zmienna gdzei mamy zapisany dzwięk,
	//loikalizacja odtwarzania dzwięku)
	
	//S5 L164
	if (OwnerController)
	{
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
		//uzyskaj punkt widzenia gracza z kontrolera i zwróc dane do zmiennych (lokalizację, rotację), jeśli jest to AI zwróci punkt widzenia z oczu modelu postaci

		//DrawDebugCamera(GetWorld(), ViewPointLocation, ViewPointRotation, 90.0f, 2.0f, FColor::Green, true);
		//narysuj debugowany kształt kamery(w tym  świecie, lokalizacja rysowanego kształtu, i rotacja, jaki kont nie jest super ważne, skala rysowanego obiektu, kolor, trwałość lini na true aby
		//znikał rysowany kształt)

		//S5 L165
		FVector EndLocation = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;
		//punkt końcowy gdzie leci pocisk = lokalizacja punktu widzenia + wektor punktu obrotu punktu widzenia * maksymalny zasięg pocisku

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		//dodajemy ignorowanych aktorów na this czyli aktora który jest utworzony z tej klasy czyli broń i właściciela broni czyli gracza

		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
		//sledzenie lini pojedynczo po kanale - zwraca true jeśli w coś trafimy(tutaj dane z trafienia, początkowy punkt lini, końcowy punkt lini, i jakiego kanału kolizji ma
		//szukać - stworzyliśmy go w edytorze Bulliet - znajdziemy jaki kanał w plikach projektu/Config/DefaultEngine.ini, zmienna ktora prszechowywuje informacje o parametrach śledzonych kanałów 
		//możemy użyć aby np zignorować niekturych aktorów)

		//S5 L166
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleParticles, MuzzleParticlesComp->GetComponentLocation(), ViewPointRotation);
		//używamy funkcji z biblioteki NIAGARA aby utworzyć system w lokalizacji(działamy w tym świecie, nasza zmienna gdzie w edytorze mamy przypisany ten system cząsteczek, lokalizacja gdzie
		//ma pojawić się ten system, i rotacja pojawiającego się efektu);

		if (IsHit)
		{
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Green, true);

			//S5 L166
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, HitResult.ImpactPoint, HitResult.ImpactPoint.Rotation());
		//używamy funkcji z biblioteki NIAGARA aby utworzyć system w lokalizacji(działamy w tym świecie, nasza zmienna gdzie w edytorze mamy przypisany ten system cząsteczek, lokalizacja gdzie
		//ma pojawić się ten system, i rotacja pojawiającego się efektu);

			//S5 L191
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
			//funkcja odtwarzająca dzwięk w danej lokalizacji(pobieramy świat że w tym świecie, zmienna gdzei mamy zapisany dzwięk, loikalizacja odtwarzania dzwięku)


			//S5 L167
			AActor* HitActor = HitResult.GetActor();
			//zmienna przechowująca trafionego aktora

			if (HitActor)
			{
				UGameplayStatics::ApplyDamage(HitActor, BulletDamage, OwnerController, this, UDamageType::StaticClass());
		//zastsuj obrażenia zadajac ogólne obrażenia aktorowi(aktor którego chcemy uszkodzić, iloś zadanych ogólnych obrażeń, inicjator wydarzenia który jest wskaźnikiem kontrolera - ten
		//kontroler jest odpowiedzialny za to uszkodzenie - ktokolwiek jest właścicielem broni jest odpowiedzialny za uszkodzenie, rzeczywisty aktor który powoduje te szkody,
		//typ obrażeń - najbardziej podstawowa wersja typu obrażeń bo nie używamy tego systemu - można tu dać obrażenia typu podpaleni, trucizna, zatrucie itp)
		//gdy uszkodzenie zostanie zastosowane do aktora uruchomiony zostanie określony delegat - OnTakeAnyDamage dalej w ShooterSamCharacter
			}

		}
	}
}
