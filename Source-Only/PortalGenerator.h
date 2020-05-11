// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PortalGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BPORTAL_API UPortalGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPortalGenerator();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UInputComponent* InputComponent = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UMaterial* DefaultMaterialOrangePortal = nullptr;
	UMaterial* DefualtMaterialBluePortal = nullptr;
	AActor* OrangePortal = nullptr;
	AActor* BluePortal = nullptr;

	UPROPERTY(EditAnywhere)
		UMaterial* OrangeMaterial = nullptr;
	
	UPROPERTY(EditAnywhere)
		UMaterial* BlueMaterial = nullptr;

	UPROPERTY(EditAnywhere)
		float PortalCastRange = 5000.f;
	
	UPROPERTY(EditAnywhere)
		float PortalEnterRange = 200.f;
	
	//setters
	void SetInputComponent();
	void SetPhysicsHandle();
	
	void CreateBluePortal();
	void CreateOrangePortal();
	//void EnterPortal();
	void GrabObject();
	void ReleaseObject();
	
	FHitResult GetActorInRange(FVector PlayerRange) const;
	FVector GetPlayerCastRange() const;
	FVector GetPlayerEnterRange() const;
	FVector GetPlayerLocation() const;

};
