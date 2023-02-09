#ifndef __PARTICLESOURCE_H__
#define __PARTICLESOURCE_H__

#include <G4VPrimaryGenerator.hh>
#include <G4PrimaryVertex.hh>
#include <G4Navigator.hh>
#include <G4ParticleMomentum.hh>
#include <G4ParticleDefinition.hh>
#include <G4Track.hh>

#include <set>

using std::set;

#include "NSParticleSourceMessenger.hh"

class NSParticleSource: public G4VPrimaryGenerator
{
public:
	NSParticleSource();
	~NSParticleSource();

public:
	void GeneratePrimaryVertex(G4Event *pEvent);
	void GeneratePrimaryVertexFromTrack(G4Track *pTrack, G4Event *pEvent);

	void SetPosDisType(G4String hSourcePosType) { mhSourcePosType = hSourcePosType; }
	void SetPosDisShape(G4String hShape) { mhShape = hShape; }
	void SetCenterCoords(G4ThreeVector hCenterCoords) { mhCenterCoords = hCenterCoords; }
	void SetHalfZ(G4double dHalfz) { mdHalfz = dHalfz; }
	void SetRadius(G4double dRadius) { mdRadius = dRadius; }

	void SetAngDistType(G4String hAngDistType) { mhAngDistType = hAngDistType; }
	void SetParticleMomentumDirection(G4ParticleMomentum hMomentum) { mhParticleMomentumDirection = hMomentum.unit(); }

	void SetEnergyDisType(G4String hEnergyDisType) { mhEnergyDisType = hEnergyDisType; }
	void SetMonoEnergy(G4double dMonoEnergy) { mdMonoEnergy = dMonoEnergy; }

	void SetParticleDefinition(G4ParticleDefinition *pParticleDefinition);
	inline void SetParticleCharge(G4double dCharge) { mdParticleCharge = dCharge; }

	void SetVerbosity(G4int iVerbosityLevel) { miVerbosityLevel = iVerbosityLevel; }

	const G4String &GetParticleType() { return mpParticleDefinition->GetParticleName(); }
	const G4double GetParticleEnergy() { return mdParticleEnergy; }
	const G4ThreeVector &GetParticlePosition() { return mhParticlePosition; }

        void GeneratePointsInPlane();
	void GeneratePointSource();
	void GeneratePointsInVolume();
	void GeneratePointsInSurface();
	void GenerateInAm241();

	G4bool IsSourceConfined();
	void ConfineSourceToVolume(G4String);

	void GenerateIsotropicFlux();

	void GenerateMonoEnergetic();

	void CreatePrimaryParticle(G4PrimaryVertex * vtx);
	void GenerateKr83mDecay(G4PrimaryVertex * vert);

private:
	G4String mhSourcePosType;
	G4String mhShape;
	G4ThreeVector mhCenterCoords;
	G4double mdHalfz;
	G4double mdRadius;
	G4bool mbConfine;
	set<G4String> mhVolumeNames;
	G4String mhAngDistType;
	G4double mdMinTheta, mdMaxTheta, mdMinPhi, mdMaxPhi;
	G4double mdTheta, mdPhi;
	G4String mhEnergyDisType;
	G4double mdMonoEnergy;

	G4int miNumberOfParticlesToBeGenerated;
	G4ParticleDefinition *mpParticleDefinition;
	G4ParticleMomentum mhParticleMomentumDirection;
	G4double mdParticleEnergy;
	G4double mdParticleCharge;
	G4ThreeVector mhParticlePosition;
	G4double mdParticleTime;
	G4ThreeVector mhParticlePolarization;

	G4int miVerbosityLevel;

	NSParticleSourceMessenger *mpMessenger;
	G4Navigator *mpNavigator;

	G4int NofPrimaries;
	G4double primEnergies[5];	
	G4double emit_energy;
	G4ParticleDefinition *particle;	
	G4ParticleTable *particleTable;
};

#endif
