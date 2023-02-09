#ifndef __EventData_H__
#define __EventData_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class NSEventData
{
public:
	 NSEventData();
	~NSEventData();

public:
	void Clear();

public:
	int                     miEventId;					// the event ID
	float                   mfTotalEnergyDeposited;		// total energy deposited in the ScintSD
	int                     miNbSteps;					// number of energy depositing steps 
	vector<int>             *mpTrackId;					// id of the particle
	vector<int>             *mpParentId;			    // id of the parent particle
	vector<string>          *mpParticleType;			// type of particle
	vector<string>          *mpParentType;				// type of parent particle
	vector<string>          *mpCreatorProcess;			// process in which the particle was created
	vector<string>          *mpDepositingProcess;		// energy depositing process
	vector<float>           *mpX;						// position of the step
	vector<float>           *mpY;
	vector<float>           *mpZ;
	vector<float>           *mpEnergyDeposited; 	    // energy deposited in the step
	vector<float>           *mpKineticEnergy;			// particle kinetic energy after the step			
	vector<float>           *mpTime;					// time of the step
	vector<string>          *mpPrimaryParticleType;		// type of particle
	float                   mfPrimaryX;				    // position of the primary particle
	float                   mfPrimaryY;
	float                   mfPrimaryZ;
	float                   mfPrimaryE;				     // energy of the primary particle
	vector<string> 		    *mpVolume;  			     // deposit energy volume, compare with geometry

	// kinetic energy and momentum
	vector<float>*  mpPreMomDirX;
	vector<float>*  mpPreMomDirY;
	vector<float>*  mpPreMomDirZ;
	vector<double>* mpPreKinNrg;

};

#endif
