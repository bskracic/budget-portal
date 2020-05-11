// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGenerator.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "BPortalGameState.h"

// Sets default values for this component's properties
UPortalGenerator::UPortalGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPortalGenerator::BeginPlay()
{
	Super::BeginPlay();

	SetInputComponent();
	SetPhysicsHandle();
}

// Called every frame
void UPortalGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector TargetLocation = GetPlayerEnterRange();

		AActor* Object = PhysicsHandle->GetGrabbedComponent()->GetAttachmentRootActor();
		FHitResult* temp = nullptr;
		TargetLocation.X = roundf(TargetLocation.X);
		TargetLocation.Y = roundf(TargetLocation.Y);
		TargetLocation.Z = roundf(TargetLocation.Z);


		Object->SetActorLocation(TargetLocation, true, temp, ETeleportType::None);
		//anulling effects of teleport
		PhysicsHandle->GetGrabbedComponent()->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f), false);
		PhysicsHandle->GetGrabbedComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f), false);
		//PhysicsHandle->SetTargetLocation(TargetLocation);
		PhysicsHandle->GetGrabbedComponent()->GetAttachmentRootActor()->bGenerateOverlapEventsDuringLevelStreaming = true;
	}
}

void UPortalGenerator::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Blue", IE_Pressed, this, &UPortalGenerator::CreateBluePortal);
		InputComponent->BindAction("Orange", IE_Pressed, this, &UPortalGenerator::CreateOrangePortal);
		InputComponent->BindAction("Grab", IE_Pressed, this, &UPortalGenerator::GrabObject);
		InputComponent->BindAction("Grab", IE_Released, this, &UPortalGenerator::ReleaseObject);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player has no Input Component!"));
	}
}

void UPortalGenerator::SetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) { UE_LOG(LogTemp, Error, TEXT("Player has no Physics Handle!")); }
}

void UPortalGenerator::CreateBluePortal()
{
	FHitResult ObjectInRange = GetActorInRange(GetPlayerCastRange());
	if (ObjectInRange.GetActor() && ObjectInRange.GetActor()->ActorHasTag("wall"))
	{
		if (ObjectInRange.GetActor() == OrangePortal) { return; }
		
		if (BluePortal)
		{	
			//remove existing blue portal
			BluePortal->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, DefualtMaterialBluePortal);
			BluePortal = nullptr;
		}
		//remove blue portal on a wall
		BluePortal = ObjectInRange.GetActor();
		ABPortalGameState::SetBluePortal(BluePortal);
		DefualtMaterialBluePortal = ObjectInRange.GetActor()->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0)->GetMaterial();
		ObjectInRange.GetActor()->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, BlueMaterial);
	}
}

void UPortalGenerator::CreateOrangePortal()
{
	FHitResult ObjectInRange = GetActorInRange(GetPlayerCastRange());
	if (ObjectInRange.GetActor() && ObjectInRange.GetActor()->ActorHasTag("wall"))
	{
		if (ObjectInRange.GetActor() == BluePortal) { return; }

		if (OrangePortal)
		{
			//remove existing orange portal
			OrangePortal->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, DefaultMaterialOrangePortal);
			OrangePortal = nullptr;
		}
		//create orange portal on a wall
		OrangePortal = ObjectInRange.GetActor();
		ABPortalGameState::SetOrangePortal(OrangePortal);
		DefaultMaterialOrangePortal = ObjectInRange.GetActor()->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0)->GetMaterial();
		ObjectInRange.GetActor()->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, OrangeMaterial);
	}
}

//void UPortalGenerator::EnterPortal()
//{
//	FHitResult ObjectInRange = GetActorInRange(GetPlayerEnterRange());
//	if (ObjectInRange.GetActor())
//	{
//		AActor* Object = ObjectInRange.GetActor();
//		//check for blue portal
//		if (Object == BluePortal)
//		{
//			if (!OrangePortal) { return; }
//			FVector OrangePortalLocation = OrangePortal->FindComponentByClass<UBoxComponent>()->GetComponentLocation();
//			FRotator OrangePortalRotation = OrangePortal->GetActorRotation();
//			FHitResult* temp = nullptr;	
//			GetOwner()->SetActorLocation(OrangePortalLocation, false, temp, ETeleportType::None);
//			GetOwner()->SetActorRotation(OrangePortalRotation, ETeleportType::None);
//		}
//		
//		//check for orange portal
//		else if (Object == OrangePortal)
//		{
//			if (!BluePortal) { return; }
//			FVector BluePortalLocation = BluePortal->FindComponentByClass<UBoxComponent>()->GetComponentLocation();
//			FRotator BluePortalRotation = BluePortal->GetActorRotation();
//			FHitResult* temp = nullptr;
//			GetOwner()->SetActorLocation(BluePortalLocation, false, temp, ETeleportType::None);
//			GetOwner()->SetActorRotation(BluePortalRotation, ETeleportType::None);
//		}
//	}
//}
	
void UPortalGenerator::GrabObject()
{
	FHitResult CollidedObject = GetActorInRange(GetPlayerEnterRange());
	if (CollidedObject.GetActor() && CollidedObject.GetActor()->ActorHasTag("grabbable"))
	{
		UPrimitiveComponent* ComponentToGrab = CollidedObject.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayerCastRange()
		);
	}
}

void UPortalGenerator::ReleaseObject()
{
	if (PhysicsHandle->GrabbedComponent)
	{	
		AActor* Object = PhysicsHandle->GetGrabbedComponent()->GetAttachmentRootActor();
		PhysicsHandle->ReleaseComponent();
		Object->bGenerateOverlapEventsDuringLevelStreaming = true;
	}
}

FVector UPortalGenerator::GetPlayerLocation() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

	return PlayerLocation;
}

FVector UPortalGenerator::GetPlayerCastRange() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

	return PlayerLocation + PlayerRotation.Vector() * PortalCastRange;
}

FVector UPortalGenerator::GetPlayerEnterRange() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

	return PlayerLocation + PlayerRotation.Vector()* PortalEnterRange;

}

FHitResult UPortalGenerator::GetActorInRange(FVector PlayerRange) const
{
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetPlayerLocation(),
		PlayerRange,
		ObjectParams,
		CollisionParams
	);

	return HitResult;
}
 
