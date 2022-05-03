#include <Randomize.hh>
#include <sys/time.h>

#include "NSRunAction.hh"
#include "NSAnalysisManager.hh"

NSRunAction::NSRunAction(NSAnalysisManager *pAnalysisManager)
{
    mAnalysisManager = pAnalysisManager;
}

NSRunAction::~NSRunAction(){}

void NSRunAction::BeginOfRunAction(const G4Run *pRun){
    if (mAnalysisManager)
        mAnalysisManager->BeginOfRun(pRun);

    struct timeval mTimeValue;
    gettimeofday(&mTimeValue,NULL);
}

void NSRunAction::EndOfRunAction(const G4Run *pRun){
    if (mAnalysisManager)
        mAnalysisManager->EndOfRun(pRun);

}
