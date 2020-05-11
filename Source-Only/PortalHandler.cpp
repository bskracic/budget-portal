// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalHandler.h"
#include "BPortalGameState.h"

// Sets default values for this component's properties
UPortalHandler::UPortalHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPortalHandler::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent = GetOwner()->FindComponentByClass<UBoxComponent>();
}

// Called every frame
void UPortalHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetOwner() == ABPortalGameState::GetBluePortal())
	{
		TArray<AActor*> OverlappingActors;
		BoxComponent->GetOverlappingActors(OverlappingActors);
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("grabbable") || Actor->ActorHasTag("beam"))
			{
				if (!(ABPortalGameState::IsOrangePortalActive())) { return; }
				AActor* OrangePortal = ABPortalGameState::GetOrangePortal();
				
				FRotator OrangePortalRotation = OrangePortal->FindComponentByClass<UBoxComponent>()->GetComponentRotation();
				FVector OrangePortalLocation = OrangePortal->FindComponentByClass<UBoxComponent>()->GetComponentLocation() + OrangePortalRotation.Vector() * 50.f;
				
				FHitResult* temp = nullptr;
				Actor->SetActorLocationAndRotation(OrangePortalLocation, OrangePortalRotation, false, temp, ETeleportType::None);
				

			}
		}

	}
	else if (GetOwner() == ABPortalGameState::GetOrangePortal())
	{
		TArray<AActor*> OverlappingActors;
		BoxComponent->GetOverlappingActors(OverlappingActors);	
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("grabbable") || Actor->ActorHasTag("beam"))
			{
				if (!(ABPortalGameState::IsBluePortalActive())) { return; }
				AActor* BluePortal = ABPortalGameState::GetBluePortal();
				
				FRotator BluePortalRotation = BluePortal->FindComponentByClass<UBoxComponent>()->GetComponentRotation();
				FVector BluePortalLocation = BluePortal->FindComponentByClass<UBoxComponent>()->GetComponentLocation() + BluePortalRotation.Vector() * 50.f;
				FHitResult* temp = nullptr;

				Actor->SetActorLocationAndRotation(BluePortalLocation, BluePortalRotation, false, temp, ETeleportType::None);
				

			}
		}
	}

}



