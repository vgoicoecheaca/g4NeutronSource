#ifndef __EVENTACTION_H__
#define __EVENTACTION_H__

#include <G4UserEventAction.hh>

#include "NSAnalysisManager.hh"

class G4Event;

class NSEventAction : public G4UserEventAction
{
public:
	NSEventAction(NSAnalysisManager *pAnalysisManager = 0);
	~NSEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	NSAnalysisManager *mAnalysisManager;
};

#endif
