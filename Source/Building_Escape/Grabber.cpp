// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Grabber works well@!@"))

    FindPhysicsHandle();
    SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Find InputComponent works well in %s"), *GetOwner()->GetName());
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Release", IE_Released, this, &UGrabber::Release);
    }
}

void UGrabber::FindPhysicsHandle()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    FHitResult HitResult = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

    if (HitResult.GetActor())
    {
        PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
    }
}

void UGrabber::Release()
{
    PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetPlayerReach() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->
                GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetPlayerReach());
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    //DrawDebugLine(
    //    GetWorld(),
    //    GetPlayerPositionAtWorld(),
    //    GetPlayerReach(),
    //    FColor(0, 255, 0),
    //    false,
    //    0.f,
    //    0,
    //    5.f
    //);

    // Ray-cast out to a certain distance(Reach)
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayerPositionAtWorld(), GetPlayerReach(),
                                            FCollisionObjectQueryParams(ECC_PhysicsBody), TraceParams);
    return Hit;
}

FVector UGrabber::GetPlayerPositionAtWorld() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->
                GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    return PlayerViewPointLocation;
}
