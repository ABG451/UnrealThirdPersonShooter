// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        //This gives the AI a basic awareness that the player exists in the scene
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        //This works too if you cast the GetActorLocation after AIPawn
        //APawn* AIPawn = GetPawn();

        //This informs the blackboard of the staring location of this AI, in case they need to reset to it
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // if (LineOfSightTo(PlayerPawn))
    // {
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    
    // // Old Code
    // //     MoveToActor(PlayerPawn, GoalStopRadius);
    // //     SetFocus(PlayerPawn);
    // }
    // else 
    // {
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // // Old Code 
    // //     ClearFocus(EAIFocusPriority::Gameplay);
    // //     StopMovement();
    // }
}

bool AShooterAIController::IsDead() const
{
    //This code returns a binary variable on whether this AI is still alive or not
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}
