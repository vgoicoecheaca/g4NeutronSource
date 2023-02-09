#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"
#include "G4StackManager.hh"
#include "G4TransportationManager.hh"


#include "NSAnalysisManager.hh"
#include "NSStackingAction.hh"

NSStackingAction::NSStackingAction(NSAnalysisManager *pAnalysisManager)
{
		mpAnalysisManager = pAnalysisManager;
}

NSStackingAction::~NSStackingAction()
{
}

G4ClassificationOfNewTrack
NSStackingAction::ClassifyNewTrack(const G4Track *pTrack)
{

	G4ClassificationOfNewTrack hTrackClassification = fUrgent;	
	// decays long-lived isotopes into the new events
	if(pTrack->GetDefinition()->GetParticleType() == "nucleus" && !pTrack->GetDefinition()->GetPDGStable())
	{
		if(pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
			hTrackClassification = fPostpone;
	}

	return hTrackClassification;
}

void
NSStackingAction::NewStage()
{
}

void
NSStackingAction::PrepareNewEvent()
{ 
}








