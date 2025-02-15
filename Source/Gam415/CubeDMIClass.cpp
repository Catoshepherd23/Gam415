// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIClass.h"
#include "GAM415Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMIClass::ACubeDMIClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates the boxComp and cubeMesh 
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");

	// Sets up boxComp and sets cubeMesh to boxComp
	RootComponent = boxComp;
	cubeMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ACubeDMIClass::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIClass::OnOverlapBegin);

	// Sees if baseMat is active and creates baseMat
	if (baseMat)
	{
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
	}

	// Sees if cubeMesh is active and sets it to dmiMat
	if (cubeMesh)
	{
		cubeMesh->SetMaterial(0, dmiMat);
	}
}

// Called every frame
void ACubeDMIClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIClass::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGam415Character* overlappedActor = Cast<AGam415Character>(OtherActor);

	// Sees if overlappedActor is active and sets the dmiMat to a random color and darkness to random NumberX
	if (overlappedActor)
	{
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		FLinearColor randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.f);

		// Sees if dmiMat is active and sets the color and darkness to a random color and random numberX
		if (dmiMat)
		{
			dmiMat->SetVectorParameterValue("Color", randColor);
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);
			
			// Checks to see if colorP is active and changes the particleComp to random color
			if (colorP)
			{
				//UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, OtherComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
				//particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
			}
		}
	}
}