#include <iostream>
#include <fstream>
#include "G4String.hh"

#include "NSDetectorConstruction.hh"
#include "NSSensitiveDetector.hh"
#include "NSMaterial.hh"
#include "NSDetectorConstructionMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Sphere.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"

#include "NSSourceConstructor.hh"
#include "NSStorage.hh"

#include <math.h>
#include <time.h>
#include<sstream>

using namespace std;

NSDetectorConstruction::NSDetectorConstruction()
    : G4VUserDetectorConstruction(){
    
	    fMessenger = new NSDetectorConstructionMessenger(this); 
    }

NSDetectorConstruction::~NSDetectorConstruction()
{
delete fMessenger;
}

G4VPhysicalVolume* NSDetectorConstruction::Construct()
{
 return ConstructVolumes();
}

G4VPhysicalVolume* NSDetectorConstruction::ConstructVolumes()
{

  fSolidWorld  = new G4Box("World_Solid",20*m,20*m,20*m);
  fLogicWorld  = new G4LogicalVolume(fSolidWorld, NSMaterial::Get()->GetAir(), "World_Logic");
  G4Colour  myWhite   (1.0, 1.0, 1.0) ;  // white
 
  fPhysicWorld = new G4PVPlacement(0,
                                 G4ThreeVector(0,0,0),
                                 "World",
                                 fLogicWorld,
                                 NULL,
                                 false,
                                 0);
    G4VisAttributes *fPhysicWorldVis = new G4VisAttributes(myWhite);

    fPhysicWorldVis->SetVisibility(false);
    fLogicWorld->SetVisAttributes(fPhysicWorldVis); 
    
    //Source 
    new NSSourceConstructor( fPhysicWorld );
    
    //Scintillator

   return fPhysicWorld;
  } 

void NSDetectorConstruction::UpdateGeometry() {
   G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}
