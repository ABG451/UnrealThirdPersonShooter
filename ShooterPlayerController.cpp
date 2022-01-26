// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
    //Adds a HUD
    HUDDisp = CreateWidget(this, HUDClass);
    if (HUDDisp != nullptr)
    {
        HUDDisp->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    //Removes HUD and and puts up the WinScreen widget if the game was won
    HUDDisp->RemoveFromViewport();
    if (bIsWinner)
    {
        UUserWidget *WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        } 
    }
    //Puts up the lose screen widget if the game was lost
    else
    {
        UUserWidget *LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        } 
    }

    
    //Restarts the level after a few seconds
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}