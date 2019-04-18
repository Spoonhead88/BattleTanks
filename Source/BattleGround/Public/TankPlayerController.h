// Prestige Games WorldWide

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:

	void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	//start moving tank barrel towards where the crosshair is pointing
	void AimTowardCrosshair();

	bool GetLookDirection(FVector2D ScreenLocation, FVector &LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector &HitLocation) const;

	//return out parameter or false
	bool GetSightRayHitLocation(FVector &HitLocation) const;

	UFUNCTION()
	void OnPossesedTankDeath();

	virtual void SetPawn(APawn *InPawn) override;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333;
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0;

	UTankAimingComponent* AimingComponent = nullptr;
};
