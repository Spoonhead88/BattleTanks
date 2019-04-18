// Prestige Games WorldWide

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SprungWheel.h"
#include "TankTrack.generated.h"

/**
 * each track applies a force to the tank
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEGROUND_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	UTankTrack();

	void SetTrackMesh();

	// set throttle between -1, +1
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	void DriveTrack(float CurrentThrottle);

	//Max Force Per Track
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 40000000; // 40 ton tank 1g acceleration

private:

	virtual void BeginPlay() override;

	TArray<class ASprungWheel*> GetWheels() const;
};
