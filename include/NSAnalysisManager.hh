#ifndef __AnalysisManager_H__
#define __AnalysisManager_H__

#include <globals.hh>

#include <TParameter.h>

class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class NSEventData;
class NSPrimaryGeneratorAction;

class NSAnalysisManager
{
public:
	NSAnalysisManager(NSPrimaryGeneratorAction *pPrimaryGeneratorAction);
	virtual ~NSAnalysisManager();

public:
	virtual void BeginOfRun(const G4Run *pRun); 
	virtual void EndOfRun(const G4Run *pRun); 
	virtual void BeginOfEvent(const G4Event *pEvent); 
	virtual void EndOfEvent(const G4Event *pEvent); 
	virtual void Step(const G4Step *pStep);	

	void SetDataFilename(const G4String &hFilename) { mhDataFilename = hFilename; }
	void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { miNbEventsToSimulate = iNbEventsToSimulate; }

private:
	G4bool FilterEvent(NSEventData *pEventData);

private:
	G4int miTargetHitsCollectionID;

	G4String mhDataFilename;
	G4int miNbEventsToSimulate;

	TFile *mpTreeFile;
	TTree *mpTree;
	TParameter<int> *mpNbEventsToSimulateParameter;

	NSPrimaryGeneratorAction *mpPrimaryGeneratorAction;

	NSEventData *mpEventData;
};

#endif
