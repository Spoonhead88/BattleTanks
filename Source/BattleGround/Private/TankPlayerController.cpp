// Prestige Games WorldWide

#include "public/TankPlayerController.h"

DECLARE_DELEGATE(Delegate)

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) { return; }

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent))
	{
		FoundAimingComponent(AimingComponent);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!GetPawn()) { return; } // return if no pawn possesed
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // out parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);

	if (bGotHitLocation)//side effect is carrying out line trace
	{
		AimingComponent->AimAt(HitLocation);
	}
}

//if linetrace hits the landscape //get the location
bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
	//find and set crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	//de project crosshair to world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//line trace along to max range and see what we hit
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

void ATankPlayerController::OnPossesedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Players OnPossesedTankDeath() Was Called. "))
	StartSpectatingOnly();
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		ATank *PossesedTank = Cast<ATank>(InPawn);
		if (!ensure(PossesedTank)) { return; }

		//subscribe our local method to the tanks death event

		// temp solution to lvalue problem
		//Delegate TmpDelegate;
		//TmpDelegate.BindLambda([this] { OnPossesedTankDeath(); });

		//original method
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossesedTankDeath);
		//PossesedTank->OnDeath.AddDynamic(this, TmpDelegate);

		//base delegate method
		/*Delegate ScriptDelegate;
		ScriptDelegate.BindUFunction(this, FName("OnPossesedTankDeath"));
		PossesedTank->OnDeath.Add(ScriptDelegate);*/
	}
}

//de project crosshair to world direction
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation,OUT FVector &LookDirection) const
{
	FVector WorldLocation; // to be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		WorldLocation, 
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera))
		{
			HitLocation = HitResult.Location;
			return true;
		}
	HitLocation = FVector(0);
	return false;
}
