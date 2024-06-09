// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gam415Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PerlinProcTerrain.h"


AGam415Projectile::AGam415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGam415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Creates object called ball mesh
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;

	// Sets the attachment for ballMesh
	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 1.0f;
}

void AGam415Projectile::BeginPlay()
{
	Super::BeginPlay();
	// Creates random color 
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);

	// Creates and sets material to projMat
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	// Assigns new ball mesh with dmiMat
	ballMesh->SetMaterial(0, dmiMat);
	// Sets dmiMat to random color
	dmiMat->SetVectorParameterValue("ProjColor", randColor);
}

void AGam415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (OtherActor != nullptr)
	{
		if (colorP)
		{
			// Gets random color if colorP is active and destroys ball when interacted with an object
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
			particleComp->SetNiagaraVariableLinearColor(FString("RandomColor"), randColor);
			ballMesh->DestroyComponent();
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		// Generate a random float between 0 and 3 for the frame number
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

		

		// Spawn a decal at the location of the hit, with a random size between 20 and 40 units, and with the normal of the hit location
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);

		// Create a dynamic material instance from the decal's material to allow for runtime changes
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		// Set the 'Color' parameter of the material instance to the randomly generated color
		MatInstance->SetVectorParameterValue("Color", randColor);

		// Set the 'Frame' parameter of the material instance to the randomly generated frame number
		MatInstance->SetScalarParameterValue("Frame", frameNum);

		APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);

		if (procTerrain)
		{
			procTerrain->AlterMesh(Hit.ImpactPoint);
		}
	}
}