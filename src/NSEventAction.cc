#include "G4Event.hh"
#include "NSEventAction.hh"

NSEventAction::NSEventAction(NSAnalysisManager *pAnalysisManager)
{
    mAnalysisManager = pAnalysisManager;
}

NSEventAction::~NSEventAction()
{
}

void NSEventAction::BeginOfEventAction(const G4Event* pEvent)
{
    if (pEvent->GetEventID() % 100000 ==0)
        std::cout << "---- Begin event " << pEvent->GetEventID() << "----" << std::endl;


    if (mAnalysisManager){
		mAnalysisManager->BeginOfEvent(pEvent);}
}
void NSEventAction::EndOfEventAction(const G4Event* pEvent)
{
    if (mAnalysisManager){
		mAnalysisManager->EndOfEvent(pEvent);}

}