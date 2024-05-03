#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init(UWorld* world, UStaticMeshComponent* mesh, const FVector& minBounds, const FVector& maxBounds);
	void Flock();

private:
    UWorld* World;
    bool initialized;
    FVector MinBounds;
    FVector MaxBounds;
    TArray<class AAgent*> Agents;

    void EnforceBoundary(AAgent* agent);
};