// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BPortalGameState.generated.h"

/**
 * 
 */
UCLASS()
class BPORTAL_API ABPortalGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
    static AActor* OrangePortal;
    static AActor* BluePortal;

public:
    static bool IsOrangePortalActive();
    static bool IsBluePortalActive();
    static void SetOrangePortal(AActor* ActorOnWhichThePortalIs);
    static void SetBluePortal(AActor* ActorOnWhichThePortalIs);
    static AActor* GetOrangePortal();
    static AActor* GetBluePortal();
};
