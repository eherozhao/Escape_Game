// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UGrabber();

protected:
    // Called when the game starts
    void BeginPlay() override;

public:
    // Called every frame
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float Reach = 100.0f;

    UPROPERTY()
    UPhysicsHandleComponent* PhysicsHandle = nullptr;

    UPROPERTY()
    UInputComponent* InputComponent = nullptr;

    void Grab();
    void Release();
    void FindPhysicsHandle();
    void SetupInputComponent();

    FHitResult GetFirstPhysicsBodyInReach() const;
    FVector GetPlayerReach() const;
    FVector GetPlayerPositionAtWorld() const;
};