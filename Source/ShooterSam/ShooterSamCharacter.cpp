// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShooterSam.h"
#include "Gun.h"
#include "ShooterSamPlayerController.h"
#include "HUDWidget.h"

AShooterSamCharacter::AShooterSamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShooterSamCharacter::BeginPlay()
{
	Super::BeginPlay();

	//S5 L167
	OnTakeAnyDamage.AddDynamic(this, &AShooterSamCharacter::OnDamageTaken);
	//powiązanie funkcji z delegatem - gdy postać otrzyma obrażenia funkcja ta zostanie wywołana

	//S5 L168
	Health = MaxHealth;

	//S5 L190
	UpdateHUD();

	//S5 L163
	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);
	//pobieramy siatkę aby ukryć kość po nazwie(nazwa kości, enum - to struktura danych PBO_None oznacza 0 czyli nic nie rób);

	//S5 L162
	Gun = GetWorld()->SpawnActor<AGun>(RifleGunClass);
	//funkcja szablonowa do tworzenia aktora<typ jakiego aktora chcemy utworzyc>(nasza zmienna UClass gdzie w edydorze ustawiamy nasz plan BP)

	if (Gun)
	{
		Gun->SetOwner(this);
		//ustawiamy właściciela broni jako ta instancja klasy postaci
		//jeśli to gracz właścicielem broni będzie klasa gracza, jeśli to wróg właścicielem broni będzie klasa wroga

		//S5 L163
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Weapon_Socket");
		//dołączamy do komponentu(jaki komponent tutaj siatka czyli kości, transformacja dołączonego elementu tutaj zatrzymaj transformację względną czyli oryginalną tego przedmiotu,
		//do jakiego miejsca w siatce dolączamy);

		//S5 L164
		Gun->OwnerController = GetController();
		//zmiennej OwnerController w klasie AGun nadajemy kontroler na początrku gry
	}
}

void AShooterSamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::Look);

		//Shoot
		//S5 L160
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AShooterSamCharacter::Shoot);
		//używając ulepszonego komponentu wejścia wywołujemy funkcję Akcja wiązania (nasz zmienna gdzie podłączymy InputAction w Edytorze, zdarzenie wywołujące co ma się stać aby to połączenie
		//zadziałało tutaj start przy naciskaniu przycisku, ten obiekt, adress funkcji którą chcemy wywołać)
	}
	else
	{
		UE_LOG(LogShooterSam, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShooterSamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	//S5 L148
	//UE_LOG(LogTemp, Warning, TEXT("MovementVector: %s"), *MovementVector.ToString());

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AShooterSamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//S5 L149
	//UE_LOG(LogTemp, Warning, TEXT("LookAxisVector: %s"), *LookAxisVector.ToString());

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AShooterSamCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AShooterSamCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AShooterSamCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AShooterSamCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AShooterSamCharacter::Shoot()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
}

void AShooterSamCharacter::UpdateHUD()
{
	//S5 L190
	AShooterSamPlayerController* PlayerController = Cast<AShooterSamPlayerController>(GetController());
	//pobieramy kontroler gracza ale naszaą klasą kontrolera gracza jest AShooterSamPlayerController więc dla tego to rzutowanie

	if (PlayerController)
	{
		float NewPercent = Health / MaxHealth;
		//dzielimy życie obecne postaci przez życie maksymalne co daje nam % i liczbę pomiędzy 0 - 1 gdzie możemy ją wykożystać
		//w HealthBar pasku zdrowia
		PlayerController->HUDWidget->SetHealthBarPercent(NewPercent);
		//po przez Player kontrolel klasy AshooterSamPlayerController używam zmiennej HUDWidget klasy AHUDWidget aby odpalić funkcję
		//którą mamy w klasie HUDWidget
	}

}

void AShooterSamCharacter::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//S5 L168
	if (IsAlive)
	{
		Health -= Damage;
		//S5 L190
		UpdateHUD();
		//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

		if (Health <= 0)
		{
			IsAlive = false;
			Health = 0.0f;
			//S5 L190
			UpdateHUD();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//pobieramy komponent kapsuły ustawiamy kolizję na wyłaćzoną(kolizja zerowa wyłączone wszystko)
			//S5 L189
			DetachFromControllerPendingDestroy();
			//odłącz od kontrolera w oczekiwaniu na zniszczenie po zabiciu odłączamy kontroler postaci

			//UE_LOG(LogTemp, Warning, TEXT("Character died: %s"), *GetActorNameOrLabel());
		}
	}

}
