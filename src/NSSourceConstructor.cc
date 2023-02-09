#include <iostream>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <fstream>
#include "G4String.hh"
#include "G4RunManager.hh" 
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4CSGSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"

#include "G4LogicalVolume.hh"

#include "G4RotationMatrix.hh"
#include "G4Point3D.hh"
#include "G4Transform3D.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "G4SubtractionSolid.hh"

#include "NSMaterial.hh"
#include "NSDetectorConstructionMessenger.hh"
#include "NSSourceConstructor.hh"
#include "NSSensitiveDetector.hh"
#include "NSMaterial.hh"
#include "NSStorage.hh"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include<sstream>

using namespace std;

NSSourceConstructor::NSSourceConstructor(G4VPhysicalVolume *myMotherVolume) {


  G4Colour  myWhite   (1.0, 1.0, 1.0) ;  // white
  G4Colour  myGray    (0.5, 0.5, 0.5) ;  // gray
  G4Colour  myBlack   (0.0, 0.0, 0.0) ;  // black
  G4Colour  myRed     (1.0, 0.0, 0.0) ;  // red
  G4Colour  myGreen   (0.0, 1.0, 0.0) ;  // green
  G4Colour  myBlue    (0.0, 0.0, 1.0) ;  // blue
  G4Colour  myCyan    (0.0, 1.0, 1.0) ;  // cyan
  G4Colour  myMagenta (1.0, 0.0, 1.0) ;  // magenta 
  G4Colour  myYellow  (1.0, 1.0, 0.0) ;  // yellow

//////////////////////////////////////////////////////////////////
// Case       -  Shield case, Pb, to stop gammas mostly 
// Holder     -  The Am241 holder, emmits the gammas and alphas
// Foam       -  To slow down the neutrons
// Au Foil    -  To moderate alpha capure in the C for neutron emission
// pellet     -  C pellet that caputes alphas then emmits neutrons
// Am241 leaf -  Radioactive source for the alphas
////////////////////////////////////////////////////////////////////

fMotherVolume = myMotherVolume;
G4double targetPhi                 = 2* pi;
G4double caseD                     = 20*mm;             
G4double caseH                     = 20*mm; 
G4double holderD                   = 15*mm;
G4double holderH                   = 10*mm;
G4double foamH                     = NSStorage::Get()->GetFoamThickness()/mm;
G4double foamD                     = 8*mm;
G4double pelletThickness           = NSStorage::Get()->GetPelletThickness()/mm;       //2*mm;
G4double modSkinThickness          = NSStorage::Get()->GetAuLeafThickness()/mm;       //10*um;
G4double am241Thickness            = NSStorage::Get()->GetAmThickness()/mm;           // 10*um
G4int numberOfLayers               = NSStorage::Get()->GetNumberOfActiveLayers();
G4double activeSourceThickness     = (pelletThickness+2*modSkinThickness+am241Thickness)*numberOfLayers + foamH + numberOfLayers*20*um;
NSStorage::Get()->SetAmRadius(foamD/2);
if (numberOfLayers>1) NSStorage::Get()->SetGenerateInAm241(1);

cout << "Au moderator thickness [mm] " 		<< modSkinThickness/mm << endl;
cout << "Pellet thickness [mm] " 		<< pelletThickness/mm << endl;
cout << "Am241 layer thickness [mm] " 		<< am241Thickness/mm  << endl;
cout << "Number of active layers "      	<< numberOfLayers << endl;
cout << "Active source total thickness [mm] "   << activeSourceThickness << endl;

if ( activeSourceThickness >=  holderH){
       	cout << "Killing because active source is larger than holder, not enough room for specified source dimensions " << endl;;
	throw std::exception();
}

/// Dummy Sensitive Screen to measure neutron (or any) rate
G4Box* solidScreen  	           = new G4Box("Screen_Solid",1*m,1*m,1*m);
G4LogicalVolume* screenLogical     = new G4LogicalVolume(solidScreen, NSMaterial::Get()->GetAir(), "Screen_Logic");
fScreenPhys 			   = new G4PVPlacement(0,G4ThreeVector(0,0,0),"Screen",screenLogical,fMotherVolume,false,0);

/////// Case ////////////
G4ThreeVector                      sourcePos(0.*cm,0.*cm,0.*cm); 
G4Tubs* caseSolid                  = new G4Tubs("CaseSolid",0,0.5*caseD,0.5*caseH,0,targetPhi*rad);
G4LogicalVolume* caseLogical       = new G4LogicalVolume(caseSolid,NSMaterial::Get()->GetPb(),"SourceCaseLogical");
fCasePhys                          = new G4PVPlacement(0,sourcePos,"Pb Source Case",caseLogical,fScreenPhys,false,0,true);

//////// Holder ////////////
G4Tubs* holderSolid                 = new G4Tubs("HolderSolid",0,0.5*holderD,0.5*holderH,0,targetPhi*rad);
G4LogicalVolume* holderLogical      = new G4LogicalVolume(holderSolid,NSMaterial::Get()->GetSteel(),"HolderLogical");
fHolderPhys                         = new G4PVPlacement(nullptr,sourcePos,"SS Holder",holderLogical,fCasePhys,false,0,true);

/////// Air bed ///////////
G4double airZOffset                 = holderH/2 - activeSourceThickness/2;
G4ThreeVector                       airPos(0,0,airZOffset);
G4Tubs* airSolid                    = new G4Tubs("AirBed",0,foamD/2+1*um,activeSourceThickness/2,0,targetPhi*rad);
G4LogicalVolume* airLogical         = new G4LogicalVolume(airSolid,NSMaterial::Get()->GetAir(),"airLogical");
fAirPhys                            = new G4PVPlacement(nullptr,airPos,"Air Bed",airLogical,fHolderPhys,false,0,true);

//////// Foam //////////////
G4double foamZOffset                = activeSourceThickness/2 - foamH/2;
G4ThreeVector                       foamPos(0.,0.,foamZOffset);
G4Tubs* foamSolid                   = new G4Tubs("foamSolid",0,0.5*foamD,0.5*foamH,0,targetPhi*rad);
G4LogicalVolume* foamLogical        = new G4LogicalVolume(foamSolid,NSMaterial::Get()->GetFoam(),"foamLogic");
fFoamPhys                           = new G4PVPlacement(0,foamPos,"Foam",foamLogical,fAirPhys,false,0,true);

///////////////////////////////////////
//// Start of the active component ////
///////////////////////////////////////
// Taking Jelenas suggestion of having several repeated layers of the Am241-Au-C Sandwich 
///// C Pellet /////////
G4double pelletH                    = pelletThickness;
G4double pelletD                    = foamD - 2*modSkinThickness;
G4double pelletZOffset              = activeSourceThickness/2 - foamH - (pelletH+4*modSkinThickness)/2;
G4ThreeVector                       pelletPos(0.,0.,pelletZOffset);
G4Tubs* pelletSolid                 = new G4Tubs("pelletSolid",0,pelletD/2,pelletH/2,0,targetPhi*rad);
G4LogicalVolume* pelletLogical      = new G4LogicalVolume(pelletSolid,NSMaterial::Get()->GetC(),"PelletLogical");

///////Au Moderator Skin//////////////
G4double modSkinD                   = foamD;
G4double modSkinInnerD              = pelletD;
G4double modSkinH                   = pelletH + 4*modSkinThickness;
G4ThreeVector                       modSkinPos(0.,0.,pelletZOffset);
G4ThreeVector                       topCap(0.,0.,modSkinH/2-modSkinThickness);
G4ThreeVector                       botCap(0.,0.,-modSkinH/2+modSkinThickness);
G4Tubs* modSkinSolid                = new G4Tubs("modSkinSolid",modSkinInnerD/2,modSkinD/2,modSkinH/2,0,targetPhi*rad);
G4Tubs* modSkinCapTopSolid          = new G4Tubs("modSkinCapTopSolid",0,modSkinD/2,modSkinThickness/2,0,targetPhi*rad);
G4Tubs* modSkinCapBottomSolid       = new G4Tubs("modSkinCapBottomSolid",0,modSkinD/2,modSkinThickness/2,0,targetPhi*rad);
G4VSolid* modSkinUnion1             = new G4UnionSolid("Side+TopCap", modSkinSolid,modSkinCapTopSolid,0,topCap);
G4VSolid* modSkinUnion2             = new G4UnionSolid("Side+Caps", modSkinUnion1,modSkinCapBottomSolid,0,botCap);
G4LogicalVolume* modLogical         = new G4LogicalVolume(modSkinUnion2,NSMaterial::Get()->GetAu(),"moderatorLogical");

///// Am241 Layer ///////
G4double AmSkinD                    = foamD;
G4double AmSkinH                    = am241Thickness;
G4double AmSkinZOffset              = pelletZOffset - (pelletH+4*modSkinThickness)/2 - AmSkinH/2;
G4ThreeVector                       AmSkinPos(0.,0.,AmSkinZOffset);
G4Tubs* AmSkinSolid                 = new G4Tubs("AmSkinSolid",0,AmSkinD/2,AmSkinH/2,0,targetPhi*rad);
G4LogicalVolume* amLogical          = new G4LogicalVolume(AmSkinSolid,NSMaterial::Get()->GetAm(),"amLogical");

G4PVPlacement* fmodSkinPhys[numberOfLayers];
G4PVPlacement* fpelletPhys[numberOfLayers];
G4PVPlacement* fAmPhys[numberOfLayers];

NSStorage::Get()->SetAmCenter(AmSkinZOffset + airZOffset);

for (int i=0;i<numberOfLayers;++i){
	char nameMod[100], namePellet[100],nameAm[100];
        sprintf(nameMod,"ModeratorSkinPhys%d",i); sprintf(namePellet,"PelletPhys%d",i); sprintf(nameAm,"Am241Phys%d",i);	
	pelletPos[2]       = (i==0) ? pelletPos[2] : pelletPos[2] - (modSkinH + AmSkinH);
	modSkinPos[2]       = (i==0) ? modSkinPos[2] : modSkinPos[2] - (modSkinH + AmSkinH);
	AmSkinPos[2]        = (i==0) ? AmSkinPos[2]  : AmSkinPos[2]  - (modSkinH + AmSkinH);	
	fpelletPhys[i]      = new G4PVPlacement(0,pelletPos, namePellet,pelletLogical,fAirPhys,  false,0,true);
	fmodSkinPhys[i]     = new G4PVPlacement(0,modSkinPos,nameMod,   modLogical,   fAirPhys,  false,0,true);
	fAmPhys[i]          = new G4PVPlacement(0,AmSkinPos, nameAm,    amLogical,    fAirPhys,  false,0,true);
}

////////////////////
// Visual Attributes
////////////////////
G4VisAttributes *fScreenVis = new G4VisAttributes();
fScreenVis->SetVisibility(false);
screenLogical->SetVisAttributes(fScreenVis); 

G4VisAttributes *fCasePhysVis = new G4VisAttributes();
fCasePhysVis->SetColour(0.,1.,1.,0.2); //cyan
fCasePhysVis->SetVisibility(true);
caseLogical->SetVisAttributes(fCasePhysVis); 

G4VisAttributes *fHolderPhysVis = new G4VisAttributes();
fHolderPhysVis->SetColour(0.,1.,0.,0.2); // green
fHolderPhysVis->SetVisibility(true);
holderLogical->SetVisAttributes(fHolderPhysVis); 

G4VisAttributes *fAirPhysVis = new G4VisAttributes();
fAirPhysVis->SetColour(0.,1.,1.,0.2); //cyan
fAirPhysVis->SetVisibility(true);
airLogical->SetVisAttributes(fAirPhysVis); 

G4VisAttributes *fFoamPhysVis = new G4VisAttributes();
fFoamPhysVis->SetColour(1.0,1.0,0.); // yellow
fFoamPhysVis->SetVisibility(true);
foamLogical->SetVisAttributes(fFoamPhysVis); 

G4VisAttributes *fAmSkinPhysVis = new G4VisAttributes();
fAmSkinPhysVis->SetColour(1.0,0.,0.); // red
fAmSkinPhysVis->SetVisibility(true);
amLogical->SetVisAttributes(fAmSkinPhysVis); 	

G4VisAttributes *fmodSkinPhysVis = new G4VisAttributes();
fmodSkinPhysVis->SetColour(0.5,0.5,0.5); // gray
fmodSkinPhysVis->SetVisibility(true);
modLogical->SetVisAttributes(fmodSkinPhysVis); 

G4VisAttributes *fPelletPhysVis = new G4VisAttributes();
fPelletPhysVis->SetColour(0,0,1.); //  blue
fPelletPhysVis->SetVisibility(true);
pelletLogical->SetVisAttributes(fPelletPhysVis); 


//////////////////////////
///Sensitive detectors////
/////////////////////////
G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
NSSensitiveDetector *pNSSensitiveDetector = new NSSensitiveDetector("NSSensitiveDetector");
pSDManager->AddNewDetector(pNSSensitiveDetector);
screenLogical->SetSensitiveDetector(pNSSensitiveDetector);
//caseLogical->SetSensitiveDetector(pNSSensitiveDetector);
//holderLogical->SetSensitiveDetector(pNSSensitiveDetector);
//foamLogical->SetSensitiveDetector(pNSSensitiveDetector);
//modLogical->SetSensitiveDetector(pNSSensitiveDetector);
//amLogical->SetSensitiveDetector(pNSSensitiveDetector);
//pelletLogical->SetSensitiveDetector(pNSSensitiveDetector);

}
