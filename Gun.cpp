// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//This sets the basic root and mesh components of the gun blueprint
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	//a sound effect and muzzle flash effect are played from the gun once when the trigger is pulled
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	//variables for calculatingt where the bullets land
	FHitResult Hit;
	FVector ShotDirection;
	bool bHitSuccess = GunTrace(Hit, ShotDirection);
	//This is true if the guntrace landed on a "shootable" object or surface
	if (bHitSuccess)
	{
		//a sound effect and muzzle flash effect are played at the location the bullet hit once 
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			//This applies damage to the actor shot by grabbing the information on which controller it was and applying the damage to them
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	//This grabs the owner of the gun, and sets it as a variable
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr) return false;
	
	//Variables to calculate the the direction and orientation of the gun raycast, based on the camera viewpoint
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	//Calculating the maximum range the raycast can extend 
	FVector End = Location + Rotation.Vector() * MaxRange;

	//Adding actors in scene to be ingonred by the raycast
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//This returns the hit inforamtion of where the raycast ended at
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const 
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}
