#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NSTargetHit.hh"

G4Allocator<TargetHit> TargetHitAllocator;

TargetHit::TargetHit() {}

TargetHit::~TargetHit()
{
	if(mpParticleType) delete mpParticleType;
	if(mpParentType) delete mpParentType;
	if(mpCreatorProcess) delete mpCreatorProcess;
	if(mpDepositingProcess) delete mpDepositingProcess;
}

TargetHit::TargetHit(const TargetHit &hTargetHit):G4VHit()
{
	mpPreMomDir = hTargetHit.mpPreMomDir;
	mpPreKinNrg = hTargetHit.mpPreKinNrg;
	miTrackId = hTargetHit.miTrackId;
	miParentId = hTargetHit.miParentId;
	mpParticleType = hTargetHit.mpParticleType;
	mpParentType = hTargetHit.mpParentType ;
	mpCreatorProcess = hTargetHit.mpCreatorProcess ;
	mpDepositingProcess = hTargetHit.mpDepositingProcess ;
	mhPosition = hTargetHit.mhPosition;
	mdEnergyDeposited = hTargetHit.mdEnergyDeposited;
	mdKineticEnergy = hTargetHit.mdKineticEnergy ;
	mdTime = hTargetHit.mdTime;
	mpVolume = hTargetHit.mpVolume;
}

const TargetHit &
TargetHit::operator=(const TargetHit &hTargetHit)
{
	mpPreMomDir = hTargetHit.mpPreMomDir;
	mpPreKinNrg = hTargetHit.mpPreKinNrg;
	miTrackId = hTargetHit.miTrackId;
	miParentId = hTargetHit.miParentId;
	mpParticleType = hTargetHit.mpParticleType;
	mpParentType = hTargetHit.mpParentType ;
	mpCreatorProcess = hTargetHit.mpCreatorProcess ;
	mpDepositingProcess = hTargetHit.mpDepositingProcess ;
	mhPosition = hTargetHit.mhPosition;
	mdEnergyDeposited = hTargetHit.mdEnergyDeposited;
	mdKineticEnergy = hTargetHit.mdKineticEnergy ;
	mdTime = hTargetHit.mdTime;
	mpVolume = hTargetHit.mpVolume;

	return *this;
}

G4int
TargetHit::operator==(const TargetHit &hTargetHit) const
{
	return ((this == &hTargetHit) ? (1) : (0));
}

void TargetHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(mhPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		//hCircle.SetTargetSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void TargetHit::Print()
{
	G4cout << "-------------------- Target hit --------------------" 
		<< "Id: " << miTrackId
		<< " Particle: " << *mpParticleType
		<< " ParentId: " << miParentId
		<< " ParentType: " << *mpParentType << G4endl
		<< "CreatorProcess: " << *mpCreatorProcess
		<< " DepositingProcess: " << *mpDepositingProcess << G4endl
		<< "Position: " << mhPosition.x()/CLHEP::mm
		<< " " << mhPosition.y()/CLHEP::mm
		<< " " << mhPosition.z()/CLHEP::mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << mdEnergyDeposited/CLHEP::keV << " keV"
		<< " KineticEnergyLeft: " << mdKineticEnergy/CLHEP::keV << " keV"
		<< " Time: " << mdTime/CLHEP::s << " s" << G4endl;
}

