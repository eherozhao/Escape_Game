// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UOpenDoor();

protected:
    // Called when the game starts
    void BeginPlay() override;

public:
    // Called every frame
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
    float InitYaw;
    float CurrentYaw;
    float MassThreshold = 50.f;

    float DoorLastOpened = 0.f;

    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 2.f;

    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;

    UPROPERTY(EditAnywhere)
    AActor* ActorThatOpen = nullptr;

    UPROPERTY(EditAnywhere)
    float OpenAngle = 0.f;

    UPROPERTY(EditAnywhere)
    float DoorOpenSpeed = 2.0f;

    UPROPERTY(EditAnywhere)
    float DoorCloseSpeed = 0.5f;

    void OpenDoor(float DeltaTime);
    void CloseDoor(float DeltaTime);
    float TotalMassOfActors() const;
    void FindAudioComponent();

    UPROPERTY()
    UAudioComponent* AudioComponent = nullptr;

    // Track whether sound plays
    bool bOpenDoorSound = false;
    bool bCloseDoorSound = false;
};
