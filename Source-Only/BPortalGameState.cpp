// Fill out your copyright notice in the Description page of Project Settings.


#include "BPortalGameState.h"

//static members
AActor* ABPortalGameState::BluePortal = nullptr;
AActor* ABPortalGameState::OrangePortal = nullptr;

//static methods
bool ABPortalGameState::IsOrangePortalActive()
{
	if (OrangePortal == nullptr)
	{
		return false;
	}
	return true;
}

bool ABPortalGameState::IsBluePortalActive()
{
	if (BluePortal == nullptr)
	{
		return false;
	}
	return true;
}

void ABPortalGameState::SetOrangePortal(AActor* ActorOnWhichPortalIs)
{
	OrangePortal = ActorOnWhichPortalIs;
}

void ABPortalGameState::SetBluePortal(AActor* ActorOnWhichPortalIs)
{
	BluePortal = ActorOnWhichPortalIs;
}

AActor* ABPortalGameState::GetOrangePortal() 
{
	return OrangePortal;
}

AActor* ABPortalGameState::GetBluePortal() 
{
	return BluePortal;
}