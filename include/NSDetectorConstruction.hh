#ifndef NSDETECTORCONTRUCTION_HH
#define NSDETECTORCONTRUCTION_HH 1 

#include "G4VUserDetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Material.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class NSDetectorConstructionMessenger;

class NSDetectorConstruction : public G4VUserDetectorConstruction
{

  public:  

    NSDetectorConstruction();   // Constructor
    ~NSDetectorConstruction();  // Destructor

    virtual G4VPhysicalVolume* Construct();
    
    void UpdateGeometry();
    void PrintDetectorParameters();

    G4VPhysicalVolume* ConstructDetector();
    NSDetectorConstructionMessenger* fDetectorMessenger;  

    inline G4LogicalVolume *GetLogWorld() {return fLogicWorld;}; 

  private:
    NSDetectorConstructionMessenger *fMessenger;
    G4VPhysicalVolume* ConstructVolumes();     

    G4double           		fWorldSizeX;
    G4double           		fWorldSizeY;
    G4double			fWorldSizeZ;
    G4Material*			fWorldMaterial;
    G4Box*			fSolidWorld;
    G4LogicalVolume*		fLogicWorld;
    G4VPhysicalVolume*		fPhysicHolder;
    G4VPhysicalVolume*		fPhysicWorld;

};

#endif
