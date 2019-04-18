// Prestige Games WorldWide

#include "TankTurret.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	//given a max elevation speed and frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}

UTankTurret::UTankTurret()
{
	PrimaryComponentTick.bCanEverTick = false;

	/*UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Tank/tank_fbx_Turret.tank_fbx_Turret'")));
	SetupAttachment(GetAttachParent(), FName("Turret"));
	SetStaticMesh(MeshAsset);
	SetRelativeLocation(GetSocketLocation(FName("Turret")));
	SetWorldScale3D(FVector(1.f));*/
}
