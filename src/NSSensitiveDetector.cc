#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>

#include <map>

#include "NSSensitiveDetector.hh"

NSSensitiveDetector::NSSensitiveDetector(G4String hName): G4VSensitiveDetector(hName)
{
	collectionName.insert("ScreenHitsCollection");
}

NSSensitiveDetector::~NSSensitiveDetector()
{
}

void NSSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent)
{
	mpTargetHitsCollection = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]);

	static G4int iHitsCollectionID = -1;

	if(iHitsCollectionID < 0)
		iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	
	pHitsCollectionOfThisEvent->AddHitsCollection(iHitsCollectionID, mpTargetHitsCollection);
	mhParticleTypes.clear();
}

G4bool NSSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory *pHistory)
{
	G4double dEnergyDeposited = pStep->GetTotalEnergyDeposit();
	G4Track *pTrack = pStep->GetTrack();
	
	G4int ID = pTrack->GetTrackID();
	//will get rid of long lived daughters, might need to fix if doing something other than AmBe
		TargetHit* pHit = new TargetHit();

		pHit->SetTrackId(pTrack->GetTrackID());

		if(!mhParticleTypes.count(pTrack->GetTrackID()))
			mhParticleTypes[pTrack->GetTrackID()] = pTrack->GetDefinition()->GetParticleName();

		pHit->SetParentId(pTrack->GetParentID());
		pHit->SetParticleType(pTrack->GetDefinition()->GetParticleName());

		if(pTrack->GetParentID())
			pHit->SetParentType(mhParticleTypes[pTrack->GetParentID()]);
		else
			pHit->SetParentType(G4String("none"));

		if(pTrack->GetCreatorProcess())
			pHit->SetCreatorProcess(pTrack->GetCreatorProcess()->GetProcessName());
		else
			pHit->SetCreatorProcess(G4String("Null"));

		    pHit->SetPreMomentumDirection(pStep->GetPreStepPoint()->GetMomentumDirection());
		    pHit->SetPreKineticEnergy(pStep->GetPreStepPoint()->GetKineticEnergy());

		pHit->SetVolume(pTrack->GetVolume()->GetName());
		pHit->SetDepositingProcess(pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
		pHit->SetPosition(pStep->GetPostStepPoint()->GetPosition());
		pHit->SetEnergyDeposited(dEnergyDeposited);
		pHit->SetKineticEnergy(pTrack->GetKineticEnergy());
		pHit->SetTime(pTrack->GetGlobalTime());

		mpTargetHitsCollection->insert(pHit);
	return true;
}

void NSSensitiveDetector::EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent)
{
}

