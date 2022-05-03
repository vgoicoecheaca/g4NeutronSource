#ifndef __STACKINGACTION_H__
#define __STACKINGACTION_H__

#include "globals.hh"
#include "G4UserStackingAction.hh"

class NSAnalysisManager;

class NSStackingAction: public G4UserStackingAction
{

public:
	NSStackingAction(NSAnalysisManager *pAnalysisManager=0);
	~NSStackingAction();
  
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	virtual void NewStage();
	virtual void PrepareNewEvent();

private:
	NSAnalysisManager *mpAnalysisManager;

};

#endif
