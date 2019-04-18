// Prestige Games WorldWide

#include "TankTrack.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "SpawnComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetTrackMesh();
	SetNotifyRigidBodyCollision(false);
	SetLinearDamping(0.5f);
	SetAngularDamping(0.8f);
	SetGenerateOverlapEvents(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//Works only once, fails the next time
	//static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> Material(TEXT("PhysicalMaterial'/Game/Tank/Track.Track'"));
	//SetPhysMaterialOverride(Material.Object);
}

void UTankTrack::SetTrackMesh()
{
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Tank/tank_fbx_Track.tank_fbx_Track'")));

	SetupAttachment(GetAttachParent(), FName("Left Track"));
	SetStaticMesh(MeshAsset);
	SetRelativeLocation(GetSocketLocation(FName("Left Track")));
	SetWorldScale3D(FVector(1.f));
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}

TArray<class ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<USpawnComponent>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) continue;

		ResultArray.Add(SprungWheel);
	}

	return ResultArray;
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel *Wheel : Wheels)
	{
		Wheel->AddDrivingforce(ForcePerWheel);
	}
}


