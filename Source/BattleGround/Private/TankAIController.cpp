// Prestige Games WorldWide

#include "Public/TankAIController.h"

DECLARE_DELEGATE(Delegate)

void ATankAIController::OnPossesedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPosessedTankDeath() Was Called. "))
		GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		ATank *PossesedTank = Cast<ATank>(InPawn);
		if (!PossesedTank) { return; }

		//subscribe our local method to the tanks death event

		// temp solution to lvalue problem
		//Delegate TmpDelegate;
		//TmpDelegate.BindLambda([this] { OnPossesedTankDeath(); });

		//original method
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossesedTankDeath);
		//PossesedTank->OnDeath.AddDynamic(this, TmpDelegate);

		//base delegate method
		/*Delegate ScriptDelegate;
		ScriptDelegate.BindUFunction(this, FName("OnPossesedTankDeath"));
		PossesedTank->OnDeath.Add(ScriptDelegate);*/
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	auto ControlledTank = GetPawn();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!(PlayerTank && ControlledTank)) { return; }

	//move toward player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// aim at player and fire
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	//when locked
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // TODO Limit fire rate
	}
}

