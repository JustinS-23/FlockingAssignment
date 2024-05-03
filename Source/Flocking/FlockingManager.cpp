#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 10    

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh, const FVector& minBounds, const FVector& maxBounds) {
    UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

    World = world;
    MinBounds = minBounds;
    MaxBounds = maxBounds;

    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);

            // Give each boid a unique random starting direction
            FVector initialDirection = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f));
            initialDirection.Normalize(); // Normalize to ensure consistent speed

            const float ConstantSpeed = 50.f;
            agent->Velocity = initialDirection * ConstantSpeed; // Apply constant speed
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::EnforceBoundary(AAgent* agent) {
    FVector location = agent->GetActorLocation();

    // Check X boundaries
    if (location.X < MinBounds.X) {
        location.X = MinBounds.X;
        agent->Velocity.X = FMath::Abs(agent->Velocity.X); // Reverse direction
    }
    else if (location.X > MaxBounds.X) {
        location.X = MaxBounds.X;
        agent->Velocity.X = -FMath::Abs(agent->Velocity.X);
    }

    // Check Y boundaries
    if (location.Y < MinBounds.Y) {
        location.Y = MinBounds.Y;
        agent->Velocity.Y = FMath::Abs(agent->Velocity.Y);
    }
    else if (location.Y > MaxBounds.Y) {
        location.Y = MaxBounds.Y;
        agent->Velocity.Y = -FMath::Abs(agent->Velocity.Y);
    }

    // Check Z boundaries
    if (location.Z < MinBounds.Z) {
        location.Z = MinBounds.Z;
        agent->Velocity.Z = FMath::Abs(agent->Velocity.Z);
    }
    else if (location.Z > MaxBounds.Z) {
        location.Z = MaxBounds.Z;
        agent->Velocity.Z = -FMath::Abs(agent->Velocity.Z);
    }

    agent->SetActorLocation(location);
}

const float CohesionWeight = 0.01f;
const float SeparationWeight = 0.5f;
const float AlignmentWeight = 0.01f;

const float ConstantSpeed = 50.f;
const float SeparationThreshold = 150.f;

void UFlockingManager::Flock() {
    if (!initialized || World == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Flocking Manager not initialized."));
        return;
    }

    for (int i = 0; i < Agents.Num(); i++) {
        AAgent* Agent = Agents[i];
        if (!Agent) continue;
        EnforceBoundary(Agent);

        FVector CohesionVector = FVector::ZeroVector;
        FVector SeparationVector = FVector::ZeroVector;
        FVector AlignmentVector = FVector::ZeroVector;

        int CohesionCount = 0;
        int SeparationCount = 0;
        int AlignmentCount = 0;

        // Iterate through other agents
        for (int j = 0; j < Agents.Num(); j++) {
            if (i == j) continue;

            AAgent* OtherAgent = Agents[j];
            FVector Direction = OtherAgent->GetActorLocation() - Agent->GetActorLocation();
            float Distance = Direction.Size();

            // Cohesion
            if (Distance > 0.f) {
                CohesionVector += OtherAgent->GetActorLocation();
                CohesionCount++;
            }

            // Separation
            if (Distance < SeparationThreshold) { // Threshold for separation
                SeparationVector -= Direction.GetSafeNormal(); // Move away
                SeparationCount++;
            }

            // Alignment
            if (Distance < 200.f) { // Threshold for alignment
                AlignmentVector += OtherAgent->Velocity; // Match velocity
                    AlignmentCount++;
            }
        }

        // Calculate final cohesion vector
        if (CohesionCount > 0) {
            CohesionVector /= CohesionCount;
            CohesionVector = (CohesionVector - Agent->GetActorLocation()).GetSafeNormal();
        }

        // Calculate final separation and alignment vectors
        if (SeparationCount > 0) {
            SeparationVector /= SeparationCount;
        }

        if (AlignmentCount > 0) {
            AlignmentVector /= AlignmentCount;
        }


        // Apply weighted vectors including attraction to center
        FVector FinalDirection = (CohesionVector * CohesionWeight) +
            (SeparationVector * SeparationWeight) +
            (AlignmentVector * AlignmentWeight);

        FinalDirection.Z = FMath::FRandRange(-1.f, 1.f);

        FinalDirection.Normalize();
        FinalDirection *= ConstantSpeed;
        Agent->Velocity = FinalDirection;

        EnforceBoundary(Agent);
    }
}
