// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
    Super::BeginPlay();
    InitYaw = GetOwner()->GetActorRotation().Yaw;
    CurrentYaw = InitYaw;

    FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (TotalMassOfActors() > MassThreshold)
    {
        OpenDoor(DeltaTime);
        DoorLastOpened = GetWorld()->GetTimeSeconds();
    }
    else
    {
        if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay)
        {
            CloseDoor(DeltaTime);
        }
    }

    ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

float UOpenDoor::TotalMassOfActors() const
{
    float TotalMass = 0.f;

    TArray<AActor*> OverlappingActors;
    if(!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set"),
            *GetOwner()->GetName());
        return TotalMass;
    }
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);


    for (AActor* OverlappingActor : OverlappingActors)
    {
        TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s is on the pressureplate"), *OverlappingActor->GetName());
    }
    return TotalMass;
}


void UOpenDoor::OpenDoor(float DeltaTime)
{
    CurrentYaw = FMath::Lerp(/*CurrentYaw, OpenAngle, 0.1*/CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);

    bCloseDoorSound = false;
    if (!AudioComponent)
    {
        return;
    }
    if (!bOpenDoorSound)
    {
        AudioComponent->Play();
        bOpenDoorSound = true;
    }
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    CurrentYaw = FMath::Lerp(/*CurrentYaw, OpenAngle, 0.1*/CurrentYaw, InitYaw, DeltaTime * DoorCloseSpeed);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);

    bOpenDoorSound = false;
    if (!AudioComponent)
    {
        return;
    }
    if (!bCloseDoorSound)
    {
        AudioComponent->Play();
        bCloseDoorSound = true;
    }
}

void UOpenDoor::FindAudioComponent()
{
    AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
    if (!AudioComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Audio Component"));
    }
}

