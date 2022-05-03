#ifndef __TargetHit_H__
#define __TargetHit_H__

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

class TargetHit: public G4VHit
{
public:
	TargetHit();
	~TargetHit();
	TargetHit(const TargetHit &);
	const TargetHit & operator=(const TargetHit &);
	G4int operator==(const TargetHit &) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:
	void SetTrackId(G4int iTrackId) { miTrackId = iTrackId; };
	void SetParentId(G4int iParentId) { miParentId = iParentId; };
	void SetParticleType(const G4String &hParticleType) { mpParticleType = new G4String(hParticleType); }
	void SetParentType(const G4String &hParentType) { mpParentType = new G4String(hParentType); }
	void SetCreatorProcess(const G4String &hProcessName) { mpCreatorProcess = new G4String(hProcessName); }
	void SetDepositingProcess(const G4String &hProcessName) { mpDepositingProcess = new G4String(hProcessName); }
	void SetPosition(G4ThreeVector hPosition) { mhPosition = hPosition; };
	void SetEnergyDeposited(G4double dEnergyDeposited) { mdEnergyDeposited = dEnergyDeposited; };
	void SetKineticEnergy(G4double dKineticEnergy) { mdKineticEnergy = dKineticEnergy; };
	void SetTime(G4double dTime) { mdTime = dTime; };
	void SetPreMomentumDirection(G4ThreeVector val){ mpPreMomDir = val;}
	void SetPreKineticEnergy(G4double val){mpPreKinNrg = val;}
	void SetVolume(const G4String &hVolume) {mpVolume = new G4String(hVolume);}

	G4int GetTrackId() { return miTrackId; };
	G4int GetParentId() { return miParentId; };
	const G4String &GetParticleType() { return *mpParticleType; }
	const G4String &GetParentType() { return *mpParentType; }
	const G4String &GetCreatorProcess() { return *mpCreatorProcess; }
	const G4String &GetDepositingProcess() { return *mpDepositingProcess; }
	G4ThreeVector GetPosition() { return mhPosition; };
	G4double GetEnergyDeposited() { return mdEnergyDeposited; };      
	G4double GetKineticEnergy() { return mdKineticEnergy; };      
	G4double GetTime() { return mdTime; };      
	G4ThreeVector GetPreMomDir(){return mpPreMomDir;}
	G4double GetPreKinEnergy(){return mpPreKinNrg;}
	const G4String &GetVolume() {return *mpVolume;}

private:
	G4int miTrackId;
	G4int miParentId;
	G4String *mpParticleType;
	G4String *mpParentType;
	G4String *mpCreatorProcess;
	G4String *mpDepositingProcess;
	G4ThreeVector mhPosition;
	G4double mdEnergyDeposited;
	G4double mdKineticEnergy;
	G4double mdTime;
	G4ThreeVector mpPreMomDir;
	G4double mpPreKinNrg;
	G4String *mpVolume;

};

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4Allocator<TargetHit> TargetHitAllocator;

inline void*
TargetHit::operator new(size_t)
{
	return((void *) TargetHitAllocator.MallocSingle());
}

inline void
TargetHit::operator delete(void *pTargetHit)
{
	TargetHitAllocator.FreeSingle((TargetHit*) pTargetHit);
}

#endif 

