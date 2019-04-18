// Prestige Games WorldWide

#include "Projectile.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("Projectile Movement Component"));
	ProjectileMovement->bAutoActivate = false;

	//collision mesh
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Game/Projectile/Shape_Sphere.Shape_Sphere'"));

	if (SphereAsset.Succeeded())
	{
		CollisionMesh->SetStaticMesh(SphereAsset.Object);
		CollisionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		CollisionMesh->SetWorldScale3D(FVector(1.f));
		CollisionMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	//set projectile material
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Projectile/M_Metal_Rust.M_Metal_Rust'"));
	
	UMaterial *MaterialObject=NULL;

	if (Material.Succeeded())
	{
		MaterialObject = Material.Object;
	}

	auto DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialObject, CollisionMesh);
	CollisionMesh->SetMaterial(0, DynamicMaterial);

	//particle sysytems
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Launch Blast"));
	LaunchBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LaunchBlast->bAutoActivate = true;
	LaunchBlast->LODMethod = ParticleSystemLODMethod::PARTICLESYSTEMLODMETHOD_ActivateAutomatic;
	LaunchBlast->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LaunchBlast->SetWorldScale3D(FVector(1.f));
	LaunchBlast->SetVisibility(true);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Impact Blast"));
	ImpactBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Projectile/LaunchBlast.LaunchBlast'"));

	if (ParticleAsset.Succeeded())
	{
		LaunchBlast->SetTemplate(ParticleAsset.Object);
		ImpactBlast->SetTemplate(ParticleAsset.Object);
	}

	// radial explosion force
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Explosion Force"));
	ExplosionForce->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionForce->Radius = 500;
	ExplosionForce->ImpulseStrength = 1000000.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();
	UE_LOG(LogTemp, Warning, TEXT("ImpulseStrength: %f"), ExplosionForce->ImpulseStrength);

	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>()
	);

	//FDamageEvent DamageEvent;
	//OtherActor->TakeDamage(ProjectileDamage, DamageEvent, OtherActor->GetOwner()->getPawn, this);

	FTimerDelegate Delegate;
	Delegate.BindLambda([this] { TimerExpired(); });

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, DestroyDelay, false);
}

void AProjectile::TimerExpired()
{
	Destroy();
}
