// Prestige Games WorldWide

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEGROUND_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	//-1 is max downward speed and +1 is max upward speed
	void Elevate(float RelativeSpeed);

private:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 5; // sensible default

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxElevationDegrees = 40; // sensible default

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MinElevationDegrees = 0; // sensible default
};
