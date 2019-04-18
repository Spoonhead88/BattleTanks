// Prestige Games WorldWide

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// how close the AI tank can get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup") 
	float AcceptanceRadius = 8000;

private:

	UFUNCTION()
	void OnPossesedTankDeath();

	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn *InPawn) override;

	UTankAimingComponent* AimingComponent = nullptr;
};
