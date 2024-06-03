// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gam415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config=Game)
class AGam415Projectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// Static mesh component for ballMesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh;

	// Material for baseMat
	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;

	UPROPERTY()
	FLinearColor randColor;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* projMat;

	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;

public:
	AGam415Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }


};

