#ifndef __PARTICLESOURCEMESSENGER_H__
#define __PARTICLESOURCEMESSENGER_H__

#include <G4UImessenger.hh>
#include <globals.hh>

class NSParticleSource;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class NSParticleSourceMessenger: public G4UImessenger
{
public:
	NSParticleSourceMessenger(NSParticleSource *pParticleSource);
	~NSParticleSourceMessenger();
  
	void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);
 
private:
	NSParticleSource *mpParticleSource;
	G4ParticleTable *mpParticleTable;
    
     G4UIdirectory              *mpDirectory;

     G4UIcmdWithAString         *mpTypeCmd;
     G4UIcmdWithAString         *mpShapeCmd;
     G4UIcmdWith3VectorAndUnit  *mpCenterCmd;
     G4UIcmdWithADoubleAndUnit  *mpHalfzCmd;
     G4UIcmdWithADoubleAndUnit  *mpRadiusCmd;
     G4UIcmdWithAString         *mpConfineCmd;         
     G4UIcmdWithAString         *mpAngTypeCmd;
     G4UIcmdWithAString         *mpEnergyTypeCmd;
     G4UIcmdWithAnInteger       *mpNumberOfParticlesToBeGeneratedCmd;
     G4UIcmdWithAnInteger       *mpVerbosityCmd;
     G4UIcommand                *mpIonCmd;
     G4UIcmdWithAString         *mpParticleCmd;
     G4UIcmdWith3VectorAndUnit  *mpPositionCmd;
     G4UIcmdWith3Vector         *mpDirectionCmd;
     G4UIcmdWithADoubleAndUnit  *mpEnergyCmd;
     G4UIcmdWithoutParameter    *mpListCmd;
     G4UIcmdWithABool           *mpAmBeCmd;

     G4bool   mbShootIon; 
     G4int    miAtomicNumber;
     G4int    miAtomicMass;
     G4int    miIonCharge;
     G4double mdIonExciteEnergy;
};

#endif
