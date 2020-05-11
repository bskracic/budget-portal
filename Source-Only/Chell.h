// Borna Skračić, Seminar 2020. Algebra
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chell.generated.h"

UCLASS()
class BPORTAL_API AChell : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChell();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void StartJump();
	
	UFUNCTION()
		void StopJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
