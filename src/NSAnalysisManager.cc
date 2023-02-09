#include <G4SDManager.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>

#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>

#include "NSDetectorConstruction.hh"
#include "NSPrimaryGeneratorAction.hh"
#include "NSEventData.hh"
#include "NSTargetHit.hh"
#include "NSAnalysisManager.hh"

NSAnalysisManager::NSAnalysisManager(NSPrimaryGeneratorAction *pPrimaryGeneratorAction)
{
	miTargetHitsCollectionID   = -1;
	mhDataFilename                = "events_output.root";
	mpPrimaryGeneratorAction      = pPrimaryGeneratorAction;
	mpEventData                   = new NSEventData();
}

NSAnalysisManager::~NSAnalysisManager()
{
}

void NSAnalysisManager::BeginOfRun(const G4Run *pRun)
{

    mpTreeFile = new TFile(mhDataFilename.c_str(), "RECREATE", "File containing event data for NS source");
	mpTree = new TTree("nstree", "Tree containing event data for Neutron Source");

	gROOT->ProcessLine("#include <vector>");
	mpTree->Branch("eventid", &mpEventData->miEventId, "eventid/I");
//	mpTree->Branch("nsteps", &mpEventData->miNbSteps, "nsteps/I");
	mpTree->Branch("etot", &mpEventData->mfTotalEnergyDeposited, "etot/F");
	//mpTree->Branch("trackid", "vector<int>", &mpEventData->mpTrackId);
	mpTree->Branch("type", "vector<string>", &mpEventData->mpParticleType);
	//mpTree->Branch("parentid", "vector<int>", &mpEventData->mpParentId);
	//mpTree->Branch("parenttype", "vector<string>", &mpEventData->mpParentType);
	//mpTree->Branch("creaproc", "vector<string>", &mpEventData->mpCreatorProcess);
	mpTree->Branch("volume", "vector<string>", &mpEventData->mpVolume);
	mpTree->Branch("edproc", "vector<string>", &mpEventData->mpDepositingProcess);
	mpTree->Branch("xp", "vector<float>", &mpEventData->mpX);
	mpTree->Branch("yp", "vector<float>", &mpEventData->mpY);
	mpTree->Branch("zp", "vector<float>", &mpEventData->mpZ);
	mpTree->Branch("ed", "vector<float>", &mpEventData->mpEnergyDeposited);
	mpTree->Branch("time", "vector<float>", &mpEventData->mpTime);
	//mpTree->Branch("type_pri", "vector<string>", &mpEventData->mpPrimaryParticleType);
	//mpTree->Branch("xp_pri", &mpEventData->mfPrimaryX, "xp_pri/F");
	//mpTree->Branch("yp_pri", &mpEventData->mfPrimaryY, "yp_pri/F");
	//mpTree->Branch("zp_pri", &mpEventData->mfPrimaryZ, "zp_pri/F");
	mpTree->Branch("mX_pre", "vector<float>", &mpEventData->mpPreMomDirX);
	mpTree->Branch("mY_pre", "vector<float>", &mpEventData->mpPreMomDirY);
	mpTree->Branch("mZ_pre", "vector<float>", &mpEventData->mpPreMomDirZ);
	//mpTree->Branch("ekin_pre", "vector<double>", &mpEventData->mpPreKinNrg);
	mpTree->SetMaxTreeSize(10e9);
	mpTree->AutoSave();

	mpNbEventsToSimulateParameter = new TParameter<int>("nbevents", miNbEventsToSimulate);
	mpNbEventsToSimulateParameter->Write();
}

void NSAnalysisManager::EndOfRun(const G4Run *pRun)
{
    mpTreeFile->Write();
    mpTreeFile->Close();
}


void NSAnalysisManager::BeginOfEvent(const G4Event *pEvent)
{
    if (miTargetHitsCollectionID == -1)
    {
        G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
        miTargetHitsCollectionID = pSDManager->GetCollectionID("ScreenHitsCollection"); 
    }
}

void NSAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
    G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
    TargetHitsCollection * pTargetHitsCollection = 0;
    G4int iNbTargetHits = 0.;

    if (pHCofThisEvent)
    {
        if (miTargetHitsCollectionID != -1)
        {
            pTargetHitsCollection = (TargetHitsCollection *)(pHCofThisEvent->GetHC(miTargetHitsCollectionID));
            iNbTargetHits         = (pTargetHitsCollection)?(pTargetHitsCollection->entries()):(0);
        }
    }

    if (iNbTargetHits)
    {
        mpEventData->miEventId = pEvent->GetEventID();
        mpEventData->mpPrimaryParticleType->push_back(mpPrimaryGeneratorAction->GetParticleTypeOfPrimary()) ;
        
        //mpEventData->mfPrimaryX = mpPrimaryGeneratorAction->GetPositionOfPrimary().x();
		//mpEventData->mfPrimaryY = mpPrimaryGeneratorAction->GetPositionOfPrimary().y();
		//mpEventData->mfPrimaryZ = mpPrimaryGeneratorAction->GetPositionOfPrimary().z();

        G4int iNbSteps = 0;

        G4float fTotalEnergyDeposited 	= 0.;
		G4float fKineticEnergy 			= 0.;

        for (G4int i=0; i<iNbTargetHits;i++)
        {
           TargetHit *pHit = (*pTargetHitsCollection)[i];                   // Need to specify what TargetHit is in a separate class!
           //if (pHit->GetParticleType() != "opticalphoton")
           //{
                mpEventData->mpTrackId->push_back(pHit->GetTrackId());
				mpEventData->mpParentId->push_back(pHit->GetParentId());
				mpEventData->mpParticleType->push_back(pHit->GetParticleType());
				mpEventData->mpVolume->push_back(pHit->GetVolume());
				
				mpEventData->mpParentType->push_back(pHit->GetParentType());
				mpEventData->mpCreatorProcess->push_back(pHit->GetCreatorProcess());
				mpEventData->mpDepositingProcess->push_back(pHit->GetDepositingProcess());

				mpEventData->mpX->push_back(pHit->GetPosition().x()/CLHEP::mm);
				mpEventData->mpY->push_back(pHit->GetPosition().y()/CLHEP::mm);
				mpEventData->mpZ->push_back(pHit->GetPosition().z()/CLHEP::mm);

				mpEventData->mpPreMomDirX->push_back(pHit->GetPreMomDir().x()/CLHEP::mm);
				mpEventData->mpPreMomDirY->push_back(pHit->GetPreMomDir().y()/CLHEP::mm);
				mpEventData->mpPreMomDirZ->push_back(pHit->GetPreMomDir().z()/CLHEP::mm);
				//mpEventData->mpPreKinNrg->push_back(pHit->GetPreKinEnergy()/CLHEP::keV);

				fTotalEnergyDeposited += pHit->GetEnergyDeposited()/CLHEP::keV;
				mpEventData->mpEnergyDeposited->push_back(pHit->GetEnergyDeposited()/CLHEP::keV);

				fKineticEnergy = pHit->GetKineticEnergy()/CLHEP::keV;
				mpEventData->mpKineticEnergy->push_back(pHit->GetKineticEnergy()/CLHEP::keV);
				mpEventData->mpTime->push_back(pHit->GetTime()/CLHEP::second);

				iNbSteps++;
           //} 
        };

        //mpEventData->miNbSteps 				= iNbSteps;
		mpEventData->mfTotalEnergyDeposited 	= fTotalEnergyDeposited;

        mpTree->Fill();
        mpEventData->Clear();

    }
}

void NSAnalysisManager::Step(const G4Step *pStep)
{}





