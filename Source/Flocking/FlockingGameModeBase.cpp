// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingGameModeBase.h"

AFlockingGameModeBase::AFlockingGameModeBase() {
    AgentMesh = CreateDefaultSubobject<UStaticMeshComponent>("AgentMesh");
    PrimaryActorTick.bCanEverTick = true;
}

void AFlockingGameModeBase::BeginPlay() {
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GAMEMODE BEGINPLAY()"));

    if (AgentMesh != nullptr) {
        Manager = NewObject<UFlockingManager>();
        if (Manager != nullptr) {

            FVector MinBounds(-750.f, -750.f, -750.f); // Minimum boundary coordinates
            FVector MaxBounds(750.f, 750.f, 750.f);   // Maximum boundary coordinates

            Manager->Init(GetWorld(), AgentMesh, MinBounds, MaxBounds);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Manager is nullptr."));
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("AgentMesh is nullptr."));
    }
}

void AFlockingGameModeBase::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    Manager->Flock();
};