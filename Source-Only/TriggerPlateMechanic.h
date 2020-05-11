// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "TriggerPlateMechanic.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BPORTAL_API UTriggerPlateMechanic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerPlateMechanic();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* TriggerPlate = nullptr;
	
	UPROPERTY(EditAnywhere)
	AActor* Plate = nullptr;

	UPROPERTY(EditAnywhere)
	float RaiseLowerPlateDelta = 5.f;

	UPROPERTY(EditAnywhere)
	UMaterial* YellowMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	float TargetZ = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorSpeed = 45;

	UPROPERTY(EditAnywhere)
	float TargetZOffSet = 30.f;

	AActor* ObjectOnPlate = nullptr;
	UMaterial* DefualtObjectMaterial = nullptr;

	bool bIsDoorOpened = false;
	bool bMaterialChanged = false;

	float InitialZ = 0.f;
	
	bool IsCubeOnPlate();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void LowerPlate();
	void RaisePlate();

};
