// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBehaviour.h"
#include "BeamProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UTurretBehaviour::UTurretBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTurretBehaviour::BeginPlay()
{
	Super::BeginPlay();
	CreateSphereDetectArea();
}


// Called every frame
void UTurretBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsPlayerInRange())
	{
		//found this through vector calculations
		FVector TargetLocation = Player->GetActorLocation() - GetOwner()->GetActorLocation();
		FRotator TargetRotation = TargetLocation.Rotation();
		GetOwner()->SetActorRotation(TargetRotation);
		if (bAbleToFire)
		{
			Fire();
			bAbleToFire = false;
			LastTimeFired = GetWorld()->GetTimeSeconds();
		}
	}

	if (Projectile)
	{
		float ElapsedTimedBetweenShots = GetWorld()->GetTimeSeconds() - LastTimeFired;
		if (ElapsedTimedBetweenShots > 2.f)
		{
			Projectile->Destroy();
			bAbleToFire = true;
		}
	}
}

void UTurretBehaviour::CreateSphereDetectArea()
{
	/*
	SphereArea = NewObject<USphereComponent>(GetOwner());
	SphereArea->SetWorldTransform(GetOwner()->GetActorTransform());
	SphereArea->SetSphereRadius(2000.f);
	SphereArea->SetVisibility(true);
	SphereArea->SetHiddenInGame(false);
	SphereArea->SetGenerateOverlapEvents(true);
	SphereArea->AttachTo(GetOwner()->GetRootComponent());
	*/
	SphereArea = GetOwner()->FindComponentByClass<USphereComponent>();
}

bool UTurretBehaviour::IsPlayerInRange()
{
	TArray<AActor*> OverlappingActors;
	SphereArea->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag("player"))
		{
			//could be stack with all objects entering its range then shooting and orienting at each one
			Player = Actor;
			return true;
		}
	}
	return false;
}

void UTurretBehaviour::Fire()
{
	if (ProjectileClass)
	{
		
		FRotator TurretRotation = GetOwner()->GetActorRotation();
		FVector TurretLocation = GetOwner()->GetActorLocation();
		TurretLocation = TurretLocation + TurretRotation.Vector() * 150.f;
		TurretLocation.Z += 25.f;
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = NULL;
			// Spawn the projectile at the muzzle.
			ABeamProjectile* Beam = World->SpawnActor<ABeamProjectile>(ProjectileClass, TurretLocation, TurretRotation, SpawnParams);
			if (Beam)
			{
				// Set the projectile's initial trajectory.
				//FVector LaunchDirection = TurretRotation.Vector();
				//Beam->FireInDirection(LaunchDirection);
				Projectile = Beam;
				//UE_LOG(LogTemp, Warning, TEXT("<%s> speed is : %f"), *Projectile->GetName(), Projectile->GetVelocity());
				//Beam->FindComponentByClass<UStaticMeshComponent>()->SetPhysicsLinearVelocity(TurretLocation * 3000, false, NAME_None);
			}
		}
	}
}

