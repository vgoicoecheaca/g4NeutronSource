#include "NSEventData.hh"

NSEventData::NSEventData()
{
    // definitions in the include file
	miEventId               = 0;
	mfTotalEnergyDeposited  = 0.;
	miNbSteps               = 0;
	mpTrackId               = new vector<int>;
	mpParentId              = new vector<int>;
	mpParticleType          = new vector<string>;
	mpParentType            = new vector<string>;
	mpCreatorProcess        = new vector<string>;
	mpDepositingProcess     = new vector<string>;
	mpX                     = new vector<float>;
	mpY                     = new vector<float>;
	mpZ                     = new vector<float>;
	mpEnergyDeposited       = new vector<float>;
	mpKineticEnergy         = new vector<float>;
	mpTime                  = new vector<float>;
	mpPrimaryParticleType   = new vector<string>;
	mfPrimaryX              = 0.;
	mfPrimaryY              = 0.;
	mfPrimaryZ              = 0.;	
	mfPrimaryE	            = 0.;
	mpPreMomDirX            = new vector<float>;
	mpPreMomDirY            = new vector<float>;
	mpPreMomDirZ            = new vector<float>;
 	mpPreKinNrg             = new vector<double>;
	mpVolume				= new vector<string>;	
}

NSEventData::~NSEventData()
{
	delete mpTrackId;
	delete mpParentId;
	delete mpParticleType;
	delete mpParentType;
	delete mpCreatorProcess;
	delete mpDepositingProcess;
	delete mpX;
	delete mpY;
	delete mpZ;
	delete mpEnergyDeposited;
	delete mpKineticEnergy;
	delete mpTime;
	delete mpPrimaryParticleType;
	delete mpPreMomDirX;
	delete mpPreMomDirY;
	delete mpPreMomDirZ;
	delete mpPreKinNrg;
	delete mpVolume;
}

void
NSEventData::Clear()
{
	miEventId = 0;
	mfTotalEnergyDeposited = 0.0;
	miNbSteps = 0;
	mpTrackId->clear();
	mpParentId->clear();
	mpParticleType->clear();
	mpParentType->clear();
	mpCreatorProcess->clear();
	mpDepositingProcess->clear();
	mpX->clear();
	mpY->clear();
	mpZ->clear();
	mpEnergyDeposited->clear();
	mpKineticEnergy->clear();
	mpTime->clear();
	mpPrimaryParticleType->clear();
	mfPrimaryX = 0.;
	mfPrimaryY = 0.;
	mfPrimaryZ = 0.;
	mfPrimaryE	= 0.;
	mpPreMomDirX->clear();
	mpPreMomDirY->clear();
	mpPreMomDirZ->clear();
	mpPreKinNrg->clear();
	mpVolume->clear();

}

