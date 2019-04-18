// Prestige Games WorldWide

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEGROUND_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	
		//-1 is max downward speed and +1 is max upward speed
		void Rotate(float RelativeSpeed);

private:
	UTankTurret();

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 20; // sensible default
};
