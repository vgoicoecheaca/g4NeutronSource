#include <globals.hh>

#include "NSPrimaryGeneratorAction.hh"
#include "NSParticleSource.hh"

#include "G4Event.hh"
#include <G4RunManagerKernel.hh>
#include <Randomize.hh>

NSPrimaryGeneratorAction::NSPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
    mpParticleSource         = new NSParticleSource();

    mhParticleTypeOfPrimary = "";
    mdEnergyOfPrimary       = 0.;
    mhPositionOfPrimary     = G4ThreeVector(0.,0.,0.);

    mlSeeds[0] = -1;
    mlSeeds[0] = -1;
}

NSPrimaryGeneratorAction::~NSPrimaryGeneratorAction()
{
    delete mpParticleSource;
}

void NSPrimaryGeneratorAction::GeneratePrimaries(G4Event* pEvent)
{
    mlSeeds[0] = *(CLHEP::HepRandom::getTheSeeds());
	mlSeeds[1] = *(CLHEP::HepRandom::getTheSeeds()+1);
	G4StackManager *pStackManager = (G4RunManagerKernel::GetRunManagerKernel())->GetStackManager();

	if(!pStackManager->GetNPostponedTrack())
	{
		mpParticleSource->GeneratePrimaryVertex(pEvent);
	}
	else
	{

		pStackManager->TransferStackedTracks(fPostpone, fUrgent);
		G4VTrajectory* pTrajectory;
		G4Track *pTrack = pStackManager->PopNextTrack(&pTrajectory);


		mpParticleSource->GeneratePrimaryVertexFromTrack(pTrack, pEvent);

		delete pTrack;
	}
	G4PrimaryVertex *pVertex = pEvent->GetPrimaryVertex();
	G4PrimaryParticle *pPrimaryParticle = pVertex->GetPrimary();

	mhParticleTypeOfPrimary = pPrimaryParticle->GetG4code()->GetParticleName();

	G4double dP = pPrimaryParticle->GetMomentum().mag();
	G4double dMass = pPrimaryParticle->GetMass();

	mdEnergyOfPrimary = std::sqrt(dP*dP + dMass*dMass) - dMass;
	mhPositionOfPrimary = pVertex->GetPosition();

}