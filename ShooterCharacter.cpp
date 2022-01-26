// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Sets initial health
	Health = MaxHealth;
	//Attaches the gun to the character
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//This assigns all the functionality to all the inputs this character can recieve
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("WeaponScrollUp"), EInputEvent::IE_Pressed, this, &AShooterCharacter::WeaponScrollUp);
	PlayerInputComponent->BindAction(TEXT("WeaponScrollDown"), EInputEvent::IE_Pressed, this, &AShooterCharacter::WeaponScrollDown);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	//This calculates the damage taken to health when a source informs this object to take damage
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Health remaining is %f"), Health);

	//This checks if this character is dead, if soit disables the dead actors collision to avoid running into the body
	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//Returns the damage that was applied
	return DamageApplied;
}


//The following functions give the character their action functionality
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::WeaponScrollUp()
{
	AddArray(ActiveWeapon);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *WeaponArray[ActiveWeapon]);
}

void AShooterCharacter::WeaponScrollDown()
{
	SubtrArray(ActiveWeapon);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *WeaponArray[ActiveWeapon]);
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

//If the the health is less or equal to zero, then this returns as true
bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


//These two functions scroll up and down an array to give which weapon is active
int32 AShooterCharacter::AddArray(int32 ArrayNum)
{
	ActiveWeapon += 1;
	if (ActiveWeapon > 3)
	{
		return ActiveWeapon = 0;
	}
	else if (ActiveWeapon < 0)
	{
		return ActiveWeapon = 3;
	}
	else
	{
		return ActiveWeapon;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%i"), ActiveWeapon);
}

int32 AShooterCharacter::SubtrArray(int32 ArrayNum)
{
	ActiveWeapon -= 1;
	if (ActiveWeapon > 3)
	{
		return ActiveWeapon = 0;
	}
	else if (ActiveWeapon < 0)
	{
		return ActiveWeapon = 3;
	}
	else
	{
		return ActiveWeapon;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%i"), ActiveWeapon);
}

