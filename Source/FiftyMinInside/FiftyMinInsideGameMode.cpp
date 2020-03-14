// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FiftyMinInsideGameMode.h"
#include "FiftyMinInsidePawn.h"

AFiftyMinInsideGameMode::AFiftyMinInsideGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AFiftyMinInsidePawn::StaticClass();

}

int AFiftyMinInsideGameMode::GetTargetCounter()
{
	return TargetCounter;
}

void AFiftyMinInsideGameMode::SetTargetCounter(int NewTargetCounter)
{
	TargetCounter = NewTargetCounter;

	if (!TargetCounter && !PickUpsCounter)
	{
		this->OnLevelCleared();
	}
}

int AFiftyMinInsideGameMode::GetPickUpsCounter()
{
	return PickUpsCounter;
}

void AFiftyMinInsideGameMode::SetPickUpsCounter(int NewPickUpsCounter)
{
	PickUpsCounter = NewPickUpsCounter;
}