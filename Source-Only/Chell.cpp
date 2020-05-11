// Borna Skračić, Seminar 2020. Algebra


#include "Chell.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AChell::AChell()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChell::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("We found a player!"));
	}

	//movement binding
	PlayerInputComponent->BindAxis("MoveForward", this, &AChell::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChell::MoveRight);
	//rotation binding(mouse controls)
	PlayerInputComponent->BindAxis("Turn", this, &AChell::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AChell::AddControllerPitchInput);
	//jump binding
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AChell::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AChell::StopJump);

}

void AChell::MoveForward(float Value)	
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AChell::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AChell::StartJump()
{
	bPressedJump = true;
}

void AChell::StopJump()
{
	bPressedJump = false;
}




