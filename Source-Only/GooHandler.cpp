// Fill out your copyright notice in the Description page of Project Settings.


#include "GooHandler.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGooHandler::UGooHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGooHandler::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent = GetOwner()->FindComponentByClass<UBoxComponent>();
	if (!BoxComponent) {
		UE_LOG(LogTemp, Error, TEXT("<%s> has no Box Component!"), *GetOwner()->GetName());
	}
	

	if (LevelToLoad == "") {
		UE_LOG(LogTemp, Error, TEXT("<%s> has no Level To Load!"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGooHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsPlayerInGoo())
	{
		UE_LOG(LogTemp, Error, TEXT("Player interacted with goo"));
	}
	
}

bool UGooHandler::IsPlayerInGoo()
{
	TArray<AActor*> OverlappingActors;
	BoxComponent->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag("player"))
		{
			UE_LOG(LogTemp, Error, TEXT("Player interacted with goo"));
			//FLatentActionInfo LatentInfo;
			//UGameplayStatics::LoadStreamLevel(this, FName("MainMenu_M") , true, true, LatentInfo);
			UGameplayStatics::OpenLevel(this, LevelToLoad);
			return true;
		}
		else if (Actor->ActorHasTag("grabbable"))
		{
			Actor->Destroy(true);
		}
	}
	return false;
}

