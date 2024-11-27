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

	//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
	// Wwise Switch & Event Calls

	UAkGameplayStatics::SetSwitch(GetImpactSwitch(HitSurfaceType), this, "", "");
	PlayBulletImpactEvent();

	//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

	// play sound
	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
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

	switch (SurfaceType)
	{		
		case SHOOTER_SURFACE_Concrete:	ImpactFX = ConcreteFX; break;
		case SHOOTER_SURFACE_Dirt:		ImpactFX = DirtFX; break;
		case SHOOTER_SURFACE_Water:		ImpactFX = WaterFX; break;
		case SHOOTER_SURFACE_Metal:		ImpactFX = MetalFX; break;
		case SHOOTER_SURFACE_Wood:		ImpactFX = WoodFX; break;
		case SHOOTER_SURFACE_Grass:		ImpactFX = GrassFX; break;
		case SHOOTER_SURFACE_Glass:		ImpactFX = GlassFX; break;
		case SHOOTER_SURFACE_Flesh:		ImpactFX = FleshFX; break;
		default:						ImpactFX = DefaultFX; break;
	}
	
	return ImpactFX;
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
// Wwise Switch Calls

UAkSwitchValue* AShooterImpactEffect::GetImpactSwitch(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UAkSwitchValue* CurrentMaterial = NULL;

	switch (SurfaceType)
	{
		case SHOOTER_SURFACE_Concrete:	
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Concrete Switch!"));
			CurrentMaterial = Concrete; 
			break;
		case SHOOTER_SURFACE_Dirt:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Water Switch!"));
			CurrentMaterial = Dirt; 
			break;
		case SHOOTER_SURFACE_Water:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Water Switch!"));
			CurrentMaterial = Water; 
			break;
		case SHOOTER_SURFACE_Metal:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Metal Switch!"));
			CurrentMaterial = Metal; 
			break;
		case SHOOTER_SURFACE_Wood:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Wood Switch!"));
			CurrentMaterial = Wood; 
			break;
		case SHOOTER_SURFACE_Grass:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Grass Switch!"));
			CurrentMaterial = Grass; 
			break;
		case SHOOTER_SURFACE_Glass:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Glass Switch!"));
			CurrentMaterial = Glass; 
			break;
		case SHOOTER_SURFACE_Flesh:		
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Flesh Switch!"));
			CurrentMaterial = Flesh; 
			break;
		default:						
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Set Default Switch!"));
			CurrentMaterial = Tile; 
			break;
	}

	return CurrentMaterial;
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

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
