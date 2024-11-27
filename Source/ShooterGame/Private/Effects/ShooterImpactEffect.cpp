// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ShooterImpactEffect.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"

AShooterImpactEffect::AShooterImpactEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAutoDestroyWhenFinished(true);
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
// Wwise Event Calls

void AShooterImpactEffect::PlayBulletImpactEvent()
{
	FOnAkPostEventCallback nullCallback;
	UAkGameplayStatics::PostEvent(BulletImpactEvent, this, int32(0), nullCallback);
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

void AShooterImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	// show particles
	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// play sound
	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		PlayBulletImpactEvent();
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (DefaultDecal.DecalMaterial)
	{
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial, FVector(1.0f, DefaultDecal.DecalSize, DefaultDecal.DecalSize),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			DefaultDecal.LifeSpan);
	}
}

UParticleSystem* AShooterImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem* ImpactFX = NULL;

	//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
	// Wwise Switch Calls

	switch (SurfaceType)
	{		
		case SHOOTER_SURFACE_Concrete:
			ImpactFX = ConcreteFX; 
			UAkGameplayStatics::SetSwitch(Concrete, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Concrete Switch!"));
			break;
		case SHOOTER_SURFACE_Dirt:		
			ImpactFX = DirtFX; 
			UAkGameplayStatics::SetSwitch(Dirt, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Dirt Switch!"));
			break;
		case SHOOTER_SURFACE_Water:		
			ImpactFX = WaterFX; 
			UAkGameplayStatics::SetSwitch(Water, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Water Switch!"));
			break;
		case SHOOTER_SURFACE_Metal:		
			ImpactFX = MetalFX; 
			UAkGameplayStatics::SetSwitch(Metal, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Metal Switch!"));
			break;
		case SHOOTER_SURFACE_Wood:		
			ImpactFX = WoodFX; 
			UAkGameplayStatics::SetSwitch(Wood, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Wood Switch!"));
			break;
		case SHOOTER_SURFACE_Grass:		
			ImpactFX = GrassFX; 
			UAkGameplayStatics::SetSwitch(Grass, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Grass Switch!"));
			break;
		case SHOOTER_SURFACE_Glass:		
			ImpactFX = GlassFX; 
			UAkGameplayStatics::SetSwitch(Glass, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Glass Switch!"));
			break;
		case SHOOTER_SURFACE_Flesh:		
			ImpactFX = FleshFX; 
			UAkGameplayStatics::SetSwitch(Flesh, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Flesh Switch!"));
			break;
		default:						
			ImpactFX = DefaultFX; 
			UAkGameplayStatics::SetSwitch(Tile, GetOwner(), "", "");
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Default Switch!"));
			break;
	}

	//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
	
	return ImpactFX;
}

USoundCue* AShooterImpactEffect::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundCue* ImpactSound = NULL;

	switch (SurfaceType)
	{
		case SHOOTER_SURFACE_Concrete:	ImpactSound = ConcreteSound; break;
		case SHOOTER_SURFACE_Dirt:		ImpactSound = DirtSound; break;
		case SHOOTER_SURFACE_Water:		ImpactSound = WaterSound; break;
		case SHOOTER_SURFACE_Metal:		ImpactSound = MetalSound; break;
		case SHOOTER_SURFACE_Wood:		ImpactSound = WoodSound; break;
		case SHOOTER_SURFACE_Grass:		ImpactSound = GrassSound; break;
		case SHOOTER_SURFACE_Glass:		ImpactSound = GlassSound; break;
		case SHOOTER_SURFACE_Flesh:		ImpactSound = FleshSound; break;
		default:						ImpactSound = DefaultSound; break;
	}

	return ImpactSound;
}
