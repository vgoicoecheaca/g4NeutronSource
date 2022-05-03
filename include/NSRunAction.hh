#ifndef __RUNACTION_H__
#define __RUNACTION_H__

#include <G4UserRunAction.hh>

class G4Run;

class NSAnalysisManager;

class NSRunAction: public G4UserRunAction
{
public:
	NSRunAction(NSAnalysisManager *pAnalysisManager=0);
	~NSRunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);

private:
	NSAnalysisManager *mAnalysisManager;
};

#endif
