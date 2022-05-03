
#ifndef NSPRIMARYGENERATORACTION_HH
#define NSPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <G4ThreeVector.hh>


class NSParticleSource;
class G4Event;


class NSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:  // Method declarations
    
    // Constructor & Destructor
    NSPrimaryGeneratorAction();
    virtual ~NSPrimaryGeneratorAction();

    const long *GetEventSeeds() { return mlSeeds; }
	  const G4String &GetParticleTypeOfPrimary() { return mhParticleTypeOfPrimary; }
	  G4double GetEnergyOfPrimary() { return mdEnergyOfPrimary; }
	  G4ThreeVector GetPositionOfPrimary() { return mhPositionOfPrimary; }
    virtual void GeneratePrimaries(G4Event* pEvent);

  private:  // Data member declarations
     long mlSeeds[2];
     G4String mhParticleTypeOfPrimary;
	   G4double mdEnergyOfPrimary;
	   G4ThreeVector mhPositionOfPrimary;
     
     NSParticleSource* mpParticleSource;
};

#endif
