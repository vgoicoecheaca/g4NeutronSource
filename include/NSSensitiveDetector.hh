#ifndef __NSSensitiveDetector_H__
#define __NSSensitiveDetector_H__

#include <G4VSensitiveDetector.hh>
#include "NSTargetHit.hh"

class G4Step;
class G4HCofThisEvent;

class NSSensitiveDetector: public G4VSensitiveDetector
{
public:
	NSSensitiveDetector(G4String hName);
	~NSSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	
	TargetHitsCollection* mpTargetHitsCollection ;
	std::map<int,G4String> mhParticleTypes;
};

#endif // __XENON10PLXESENSITIVEDETECTOR_H__

