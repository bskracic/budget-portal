// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeamProjectile.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "TurretBehaviour.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BPORTAL_API UTurretBehaviour : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurretBehaviour();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void CreateSphereDetectArea();
	void Fire();
	bool IsPlayerInRange();
	
	USphereComponent* SphereArea = nullptr;
	AActor* Player = nullptr;
	AActor* Projectile = nullptr;
	float LastTimeFired = 0.f;
	bool bAbleToFire = true;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABeamProjectile> ProjectileClass;
	
};
