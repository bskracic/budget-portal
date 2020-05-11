// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/TriggerVolume.h"
#include "TriggerPlateMechanic.h"
#include "Materials/Material.h"


// Sets default values for this component's properties
UTriggerPlateMechanic::UTriggerPlateMechanic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerPlateMechanic::BeginPlay()
{
	Super::BeginPlay();

	if (!TriggerPlate) { UE_LOG(LogTemp, Error, TEXT("Door has no Trigger Plate")); }
	if (!Plate) { UE_LOG(LogTemp, Error, TEXT("Door has no Plate")); }
	InitialZ = GetOwner()->GetActorLocation().Z;
	TargetZ = InitialZ + TargetZ;
}

// Called every frame
void UTriggerPlateMechanic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsCubeOnPlate())
	{
		if (!bIsDoorOpened) 
		{
			OpenDoor(DeltaTime);
			if (ObjectOnPlate->ActorHasTag("player")) { return; }
			if (!bMaterialChanged)
			{
				DefualtObjectMaterial = ObjectOnPlate->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0)->GetMaterial();
				ObjectOnPlate->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, YellowMaterial);
				bMaterialChanged = true;
				LowerPlate();
			}
		}
	}
	else
	{
		if (bIsDoorOpened) 
		{
			CloseDoor(DeltaTime);
			if (ObjectOnPlate->ActorHasTag("player")) { return; }
			if (bMaterialChanged)
			{
				ObjectOnPlate->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, DefualtObjectMaterial);
				bMaterialChanged = false;
				RaisePlate();
			}
		}
	}
}

bool UTriggerPlateMechanic::IsCubeOnPlate()
{
	if (!TriggerPlate) { return false; }
	
	TArray<AActor*> OverlappingActors;
	TriggerPlate->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag("grabbable"))
		{
			ObjectOnPlate = Actor;
			return true;
		}
	}
	return false;
}

void UTriggerPlateMechanic::OpenDoor(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Open Door"));
	FVector Location = GetOwner()->GetActorLocation();
	float ModifiedZ = FMath::Lerp(Location.Z, TargetZ, DeltaTime*DoorSpeed);
	Location.Z = ModifiedZ;
	GetOwner()->SetActorLocation(Location);

	UE_LOG(LogTemp, Warning, TEXT("Height: %f, Target: %f"), GetOwner()->GetActorLocation().Z, TargetZ);
	if (Location.Z >= TargetZ - TargetZOffSet) { bIsDoorOpened = true; }
	
}

void UTriggerPlateMechanic::CloseDoor(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Closing Door"));
	FVector Location = GetOwner()->GetActorLocation();
	float ModifiedZ = FMath::Lerp(Location.Z, InitialZ, DeltaTime*DoorSpeed);
	Location.Z = ModifiedZ;
	GetOwner()->SetActorLocation(Location);

	UE_LOG(LogTemp, Warning, TEXT("Height: %f, Target: %f"), GetOwner()->GetActorLocation().Z, InitialZ);
	if (Location.Z <= InitialZ + TargetZOffSet ) { bIsDoorOpened = false; }
}

void UTriggerPlateMechanic::LowerPlate()
{
	FVector PlateLocation = Plate->GetActorLocation();
	PlateLocation.Z -= RaiseLowerPlateDelta;
	FHitResult* temp = nullptr;
	Plate->SetActorLocation(PlateLocation, false, temp, ETeleportType::None);
}

void UTriggerPlateMechanic::RaisePlate()
{
	FVector PlateLocation = Plate->GetActorLocation();
	PlateLocation.Z += RaiseLowerPlateDelta;
	FHitResult* temp = nullptr;
	Plate->SetActorLocation(PlateLocation, false, temp, ETeleportType::None);
}
