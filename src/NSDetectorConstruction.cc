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

#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"

#include <math.h>
#include <time.h>
#include<sstream>

using namespace std;

NSDetectorConstruction::NSDetectorConstruction()
    : G4VUserDetectorConstruction()
{
 //Define variables here because some will be modified by messenger, if they are in the constuctor they will get overwritten;
 outerShieldX = 25*cm;
 outerShieldY = 25*cm;
 outerShieldZ = 25*cm;
 innerShieldX = 2.5*cm;
 innerShieldY = 2.5*cm;
 innerShieldZ = 2.5*cm;
 targetD      = 8.71*cm;
 targetH      = 9.01*cm;
 targetPhi    = 2* pi;
 acDiskD      = 18.8*cm;         
 acDiskH      = 4*1.2*cm; 
 alDiskD      = 17.5*cm; 
 alDiskH      = 2.7*cm; 
 outerGapX    = innerShieldX+targetH+alDiskH+(acDiskH*6); 
 outerGapY    = innerShieldY+targetH+alDiskH+(acDiskH*6); 
 outerGapZ    = innerShieldZ+targetH+alDiskH+(acDiskH*6); 
 innerGapX    = targetH+alDiskH+(acDiskH*6);
 innerGapY    = targetH+alDiskH+(acDiskH*6);
 innerGapZ    = targetH+alDiskH+(acDiskH*6);
 windowD      = 3*cm;    
 windowH      = 2*cm;
 sourceGapZ   = (acDiskH-windowH)*0.5;                      // with respect to the center of the acrylic disk
 sourceGap0   = targetH*0.5+acDiskH+alDiskH-(windowH*0.5);  //for later use in the source placement 

 caseD         = 20*mm; 
 caseH         = 15*mm; 
 holderD       = 14*mm;
 holderH       = 9*mm;
 holderGapD    = 8*mm;
 holderGapH    = 5*mm;  
 sourceH       = 4*mm;
 moderatorH    = 2*micrometer;
 foamD         = holderGapD;
 foamH         = 1*mm; 
 pelletD       = holderGapD; 
 pelletH       = 2*mm;
 /// need to refix som of this
 //sourcePos     = G4ThreeVector(0.,0.,sourceGap0); 
 sourcePos     = G4ThreeVector(0.,0.,0*cm); 
 sourceGapPos  = G4ThreeVector(0.,0.,(holderGapH*0.5)-(holderH*0.5));
 am241pos      = sourcePos + G4ThreeVector(0,0,  -(holderH*0.5)+foamH+sourceH*0.5);
 sourceFilmPos = sourcePos + G4ThreeVector(0.,0.,-(holderH*0.5)+foamH+pelletH+sourceH*0.5);
 modPosBel     = sourcePos + G4ThreeVector(0.,0.,-(holderH*0.5)+foamH+pelletH);
 pelletPos     = sourcePos + G4ThreeVector(0.,0.,-(holderH*0.5)+foamH+(pelletH*0.5));
 modPosAb      = sourcePos + G4ThreeVector(0.,0.,-(holderH*0.5)+foamH);
 foamPos       = sourcePos + G4ThreeVector(0.,0.,-(holderH*0.5)+(foamH*0.5));
 pDetectorMessenger = new NSDetectorConstructionMessenger(this);
}

NSDetectorConstruction::~NSDetectorConstruction()
{
delete pDetectorMessenger;
}

G4VPhysicalVolume* NSDetectorConstruction::Construct()
{
 return ConstructVolumes();
}

G4VPhysicalVolume* NSDetectorConstruction::ConstructVolumes()
{

    // cleanup old geometry
   G4GeometryManager::GetInstance()->OpenGeometry();
   G4PhysicalVolumeStore::GetInstance()->Clean();
   G4LogicalVolumeStore::GetInstance()->Clean();
   G4SolidStore::GetInstance()->Clean();

   // Define the Lab 
   G4double labX = 5*m;
   G4double labY = 5*m;
   G4double labZ = 5*m;
   G4Box* labSolid   = new G4Box("solid-world",0.5*labX,0.5*labY,0.5*labZ);
   labLogical        = new G4LogicalVolume(labSolid,NSMaterial::Get()->GetAir(),"logic-World");
   mLabPhysical      = new G4PVPlacement(0,G4ThreeVector(),labLogical,"World",0,false,0,true);


    //////////////////////////////////
    ///START OF GEOMETRY DEFINITION///
    /////////////////////////////////

   // Pb outer shield
   G4Box* outerShieldSolid             = new G4Box("solid-outerShield",  0.5*(outerShieldX+outerGapX),0.5*(outerShieldY+outerGapY),                                        0.5*(outerShieldZ+outerGapZ));
   G4Box* outerGapSolid                = new G4Box("solid-outerGap",0.5*outerGapX,0.5*outerGapY,0.5*outerGapZ); 
   G4SubtractionSolid* outerSubSolid   = new G4SubtractionSolid("solid-outerShieldGap",outerShieldSolid, outerGapSolid);
   G4LogicalVolume* outerShieldLogical = new G4LogicalVolume(outerSubSolid,NSMaterial::Get()->GetPb(),"logic-outerShield");    
   mOuterShieldPhysical                = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,0.),outerShieldLogical,"OuterShield",labLogical,false,0,true);

   // Cu inner shield
   G4Box* innerShieldSolid             = new G4Box("solid-innerShield", 0.5*(innerShieldX+innerGapX),0.5*(innerShieldY+innerGapY),                                       0.5*(innerShieldZ+innerGapZ));
   G4Box* innerGapSolid                = new G4Box("solid-innerGap", 0.5*innerGapX, 0.5*innerGapY, 0.5*innerGapZ); 
   G4SubtractionSolid* innerSubSolid   = new G4SubtractionSolid("solid-innerShieldGap",innerShieldSolid, innerGapSolid);
   G4LogicalVolume* innerShieldLogical = new G4LogicalVolume(innerSubSolid, NSMaterial::Get()->GetCu(), "logic-innerShield");    
   mInnerShieldPhysical                = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,0.),innerShieldLogical,"InnerShield",labLogical,false,0,true);

    // Acrylic disks, with source hole 
    acDiskSolid       = new G4Tubs("solid-acDisk",0,acDiskD*0.5,acDiskH*0.5,0,targetPhi*rad);
    windowSolid       = new G4Tubs("solid-window",0,windowD*0.5,windowH*0.5,0,targetPhi*rad); 
    acDiskWindowSolid = new G4SubtractionSolid("solid-acDiskWindow",acDiskSolid,windowSolid,nullptr,G4ThreeVector(0.,0.,sourceGapZ));
    acDiskLogic       = new G4LogicalVolume(acDiskWindowSolid,NSMaterial::Get()->GetAcrylic(),"logic-acDisk");
    //mAcDiskPhysical = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,(targetH+acDiskH)*0.5+alDiskH),acDiskLogic,"AcrylicDisk",labLogical,false,0,true);

    // Aluminum disks
    alDiskSolid = new G4Tubs("solid-alDisk", 0, alDiskD*0.5, alDiskH*0.5, 0, targetPhi*rad);
    alDiskLogic = new G4LogicalVolume(alDiskSolid, NSMaterial::Get()->GetAl(), "logic-alDisk");
    //mAlDiskPhysical = new G4PVPlacement(nullptr,G4ThreeVector(0,0.,(targetH+alDiskH)*0.5),alDiskLogic,"AlDisk",labLogical,false,0,true);

    // Ge Target
    G4Tubs* targetSolid            = new G4Tubs("solid-target",  0,targetD*0.5,targetH*0.5,0,targetPhi*rad);
    G4LogicalVolume* targetLogical = new G4LogicalVolume(targetSolid,NSMaterial::Get()->GetWater(),"logic-target");    
    G4RotationMatrix* rotTarget    = new G4RotationMatrix();
    rotTarget->rotateX(90*deg);
    mTargetPhysical                = new G4PVPlacement(rotTarget,G4ThreeVector(0.,0.,-7*cm),targetLogical,"Target",labLogical,false,0,true);

    // for the source, height is chosen such that it will fit, change parameters accordingly once design has been finalized
    // the radius is more critical and is to be chosen such that it will fit withih the pipes

    // source holder, pb
    G4Tubs* sourceCaseSolid            = new G4Tubs("solid-sourceCase",0,0.5*caseD,0.5*caseH,0,targetPhi*rad);
    G4Tubs* sourceCaseGapSolid         = new G4Tubs("solid-sourceCaseGap",0,holderD*0.5,holderH*0.5,0,targetPhi*rad);
    G4SubtractionSolid* caseSolid      = new G4SubtractionSolid("sourceCasewGap",sourceCaseSolid,sourceCaseGapSolid);
    G4LogicalVolume* sourceCaseLogical = new G4LogicalVolume(caseSolid,NSMaterial::Get()->GetPb(),"logic-sourceCase");
    mSourceCasePhysical                = new G4PVPlacement(nullptr,sourcePos,sourceCaseLogical,"LeadSourceCase",labLogical,false,0,true);

    //Am241 source along with the stainless steel holder
    G4Tubs* holderSolid                 = new G4Tubs("solid-holderFull",0,0.5*holderD,0.5*holderH,0,targetPhi*rad);
    G4Tubs* holderGap                   = new G4Tubs("solid-holderGap",0,holderGapD*0.5,holderGapH*0.5,0,targetPhi*rad);
    G4SubtractionSolid* holderSubSolid  = new G4SubtractionSolid("solid-holderSteel",holderSolid,holderGap,nullptr,sourceGapPos); 
    G4LogicalVolume* holderLogical      = new G4LogicalVolume(holderSubSolid,NSMaterial::Get()->GetSteel(),"logic-holderSteel");
    mHolderPhysical                     = new G4PVPlacement(nullptr,sourcePos,holderLogical,"SteelHolder",labLogical,false,0,true);
    
    cout << "the source position is (am241) " << am241pos[2] << endl;   

    G4Tubs* amSolid            = new G4Tubs("solid-source",0,0.5*holderGapD,sourceH*0.5,0,targetPhi*rad);
    G4LogicalVolume* amLogical = new G4LogicalVolume(amSolid,NSMaterial::Get()->GetAm(),"logic-Am241");
    //mAmPhysical                = new G4PVPlacement(nullptr,am241pos,amLogical,"Am241", labLogical,false,0,true);

    // Foam spacer
    G4Tubs* foamSolid            = new G4Tubs("solid-foam",0,0.5*foamD,0.5*foamH,0,targetPhi*rad);
    G4LogicalVolume* foamLogical = new G4LogicalVolume(foamSolid,NSMaterial::Get()->GetFoam(),"logic-foam");

    // pellet surrounded by a gold sheet moderator
    //G4Tubs* moderatorSolid = new G4Tubs("solid-moderator",0,0.5*(pelletD+moderatorT),0.5*(pelletH+moderatorT),0,targetPhi*rad);
    //G4SubtractionSolid* moderatorSubSolid  = new G4SubtractionSolid("solid-moderator",moderatorSolid,moderatorGapSolid); 
    //moderatorSolid   = new G4Tubs("solid-moderator",0,0.5*pelletD,moderatorH*0.5,0,targetPhi*rad);
    //moderatorLogical = new G4LogicalVolume(moderatorSolid,NSMaterial::Get()->GetAu(),"logic-moderator");
    //mModeratorPhysical_bel = new G4PVPlacement(nullptr,modPosBel,moderatorLogical,"GoldModeratorBottom",labLogical,false,0,true);
    //mModeratorPhysical_ab  = new G4PVPlacement(nullptr,modPosAb,moderatorLogical,"GoldModeratorTop",labLogical,false,0,true);

    pelletSolid     = new G4Tubs("solid-pellet",0,0.5*pelletD,0.5*pelletH,0,targetPhi*rad);
    mPelletLogical  = new G4LogicalVolume(pelletSolid,NSMaterial::Get()->GetBe(),"logic-pellet");
    mPelletPhysical = new G4PVPlacement(nullptr,am241pos,mPelletLogical,"Pellet",labLogical,false,0,true);

    ////////////////////////
    /// Visual Atributes ///
    /////////////////////// 
   
    G4Colour white  (1.0,	1.0,	1.0, 0.7);
	G4Colour grey   (0.5,	0.5,	0.5, 0.7);
	G4Colour lgrey  (.85,	.85,	.85, 0.7);
	G4Colour red    (1.0,	0.0,	0.0, 0.7);
	G4Colour lred   (0.75,	0.0,	0.,  0.7);
	G4Colour xlred  (0.5,	0.0,	0.0, 0.7);
	G4Colour cyan   (0.0,	1.0,	1.0, 0.7);
	G4Colour blue   (0.0,	0.0,	1.0, 0.7);
	G4Colour lblue  (.5,	0.5,	1.,  1.);
	G4Colour xlblue (.5,	0.5,	1.,	 0.2);
	G4Colour magenta(1.0,	0.0,	1.0, 0.7);
	G4Colour yellow (1.0,	1.0,	0.0, 0.7);
	G4Colour green  (0.,	.1,		0.0, 0.7);
	G4Colour lgreen (0.0,	.75,	0.0, 0.7);
	G4Colour xlgreen(0.0,	0.5,	0.0, 0.7);
	G4Colour brown  (0.7,	0.4,	0.1, 0.7);
	G4Colour orange (1.0,	0.5,	0.0, 0.7);
	G4Colour xlorange (1.0,	0.5,	0.0, 0.5);

    G4VisAttributes *outerShieldVis = new G4VisAttributes(cyan);
	outerShieldVis->SetVisibility(true);
	outerShieldLogical->SetVisAttributes(outerShieldVis); 

    G4VisAttributes *innerShieldVis = new G4VisAttributes(orange);
	innerShieldVis->SetVisibility(true);
	innerShieldLogical->SetVisAttributes(innerShieldVis);  

    G4VisAttributes *acDiskVis = new G4VisAttributes(lgrey);
	acDiskVis->SetVisibility(true);
    acDiskLogic->SetVisAttributes(acDiskVis); 

    G4VisAttributes *alDiskVis = new G4VisAttributes(lgreen);
	alDiskVis->SetVisibility(true);
    alDiskLogic->SetVisAttributes(alDiskVis); 

    G4VisAttributes *targetVis = new G4VisAttributes(magenta);
	targetVis->SetVisibility(true);
    targetLogical->SetVisAttributes(targetVis); 

    G4VisAttributes *sourceCaseVis = new G4VisAttributes(cyan);
	sourceCaseVis->SetVisibility(true);
	sourceCaseLogical->SetVisAttributes(sourceCaseVis); 

    G4VisAttributes *sourceHolderVis = new G4VisAttributes(xlorange);
	sourceHolderVis->SetVisibility(true);
	holderLogical->SetVisAttributes(sourceHolderVis); 

    G4VisAttributes *foamVis = new G4VisAttributes(yellow);
	foamVis->SetVisibility(true);
	foamLogical->SetVisAttributes(foamVis); 
   
    G4VisAttributes *am241Vis = new G4VisAttributes(brown);
	am241Vis->SetVisibility(true);
	amLogical->SetVisAttributes(am241Vis); 

//    G4VisAttributes *moderatorVis = new G4VisAttributes(red);
//	moderatorVis->SetVisibility(true);
//	moderatorLogical->SetVisAttributes(moderatorVis); 
    
    G4VisAttributes *pelletVis = new G4VisAttributes(white);
	pelletVis->SetVisibility(true);
	mPelletLogical->SetVisAttributes(pelletVis); 

    // Invisible volumes
	labLogical->SetVisAttributes(G4VisAttributes::GetInvisible());		
    innerShieldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
    outerShieldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
    acDiskLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
    alDiskLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
    //targetLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
    //sourceHolderLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
    //foamLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
    //am241Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
    //mModeratorPhysical->SetVisAttributes(G4VisAttributes::GetInvisible());
    //pelletLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

    //////////////////////////////////
    /// END OF GEOMETRY DEFINITIONS///
    //////////////////////////////////
    /// define the sensitive detector
    G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
    NSSensitiveDetector *pTargetSD = new NSSensitiveDetector("TargetSD");
    pSDManager->AddNewDetector(pTargetSD);
    targetLogical->SetSensitiveDetector(pTargetSD);  
    //outerShieldLogical->SetSensitiveDetector(pTargetSD); 
    //innerShieldLogical->SetSensitiveDetector(pTargetSD);
    //acDiskLogic->SetSensitiveDetector(pTargetSD);
    //alDiskLogic->SetSensitiveDetector(pTargetSD);
    //sourceCaseLogical->SetSensitiveDetector(pTargetSD);
    //holderLogical->SetSensitiveDetector(pTargetSD);
    //amLogical->SetSensitiveDetector(pTargetSD);
    //foamLogical->SetSensitiveDetector(pTargetSD);
    //moderatorLogical->SetSensitiveDetector(pTargetSD);
    //mPelletLogical->SetSensitiveDetector(pTargetSD);

    return mLabPhysical;

}

// Funcitons for object and source placement

void NSDetectorConstruction::SetWindowD(G4double value)
{                            
    windowD = value;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NSDetectorConstruction::SetWindowH(G4double value)
{
    windowH = value;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NSDetectorConstruction::SetSourcePos(G4double value)
{
    sourcePos = G4ThreeVector(0,0,value); 
    am241pos  = G4ThreeVector(0,0, (holderH*0.5)+pelletH+foamH+sourceH*0.5);
    pelletPos = G4ThreeVector(0,0, (holderH*0.5)+pelletH*0.5+foamH);
    cout << "The Am241 center position is " << value << endl; 
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

void NSDetectorConstruction::PlaceAlDisk(G4bool value)
{
    if (value){
        mAlDiskPhysical = new G4PVPlacement(nullptr,G4ThreeVector(0,0.,(targetH+alDiskH)*0.5),alDiskLogic,"AlDisk",labLogical,false,0,true);
        G4RunManager::GetRunManager()->ReinitializeGeometry(); 
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}

void NSDetectorConstruction::PlaceAcDisk(G4bool value)
{
    if (value){
        mAcDiskPhysical = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,(targetH+acDiskH)*0.5+alDiskH),acDiskLogic,"AcrylicDisk",labLogical,false,0,true);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
        G4RunManager::GetRunManager()->ReinitializeGeometry(); 
    }
}

void NSDetectorConstruction::SetPelletH(G4double value)
{
    sourceH = value;      
    am241pos      = sourcePos + G4ThreeVector(0,0, -(holderH*0.5)+foamH+sourceH*0.5);
    cout << "the source position is "<< am241pos[2]<< endl;
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

void NSDetectorConstruction::SetCaseD(G4double value){
    caseD = value;
    cout << "Pb Case Diameter set to "<< caseD << ", the total Pb thickness is now (mm) "<< (caseD - holderD) << endl;
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

void NSDetectorConstruction::SetCaseH(G4double value){
    caseH = value;
    cout << "Pb Case height set to "<< caseH << ", the total Pb thickness is now (mm) "<< (caseH - holderH)/2 << endl;
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

void NSDetectorConstruction::PlacePellet(G4bool value){
    if (value){
       cout << "Hello ther" << endl;
       // mPelletPhysical = new G4PVPlacement(nullptr,pelletPos,mPelletLogical,pelletName,labLogical,false,0,true);
       // G4RunManager::GetRunManager()->ReinitializeGeometry(); 
       // G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}

void NSDetectorConstruction::PlaceFoam(G4bool value){
    if (value){
        mFoamPhysical = new G4PVPlacement(nullptr,foamPos,foamLogical,"Foam",labLogical,false,0,true);
        G4RunManager::GetRunManager()->ReinitializeGeometry(); 
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}

void NSDetectorConstruction::SetSourceMaterial(G4String value)
{
    if(value=="AmBe"){
        G4cout << "source set to AmBe" << G4endl;
        pelletName = "BePellet";
        mPelletLogical->SetMaterial(NSMaterial::Get()->GetBe());
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();}
        //G4RunManager::GetRunManager()->ReinitializeGeometry();}
    else if (value=="AmC"){
        G4cout << "source set to AmC" << G4endl; 
        pelletName = "CPellet";
        mPelletLogical->SetMaterial(NSMaterial::Get()->GetC());
        mModeratorPhysical_bel = new G4PVPlacement(nullptr,modPosBel,moderatorLogical,"GoldModeratorBottom",labLogical,false,0,true);
        mModeratorPhysical_ab  = new G4PVPlacement(nullptr,modPosAb,moderatorLogical,"GoldModeratorTop",labLogical,false,0,true);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();}
        //G4RunManager::GetRunManager()->ReinitializeGeometry();}
    else{
        G4cout << "No know source Material, please change" << G4endl;}
}

void NSDetectorConstruction::SetTargetMaterial(G4String value){
    if(value=="Ge"){
        G4cout << "Target Material is set to Ge" << G4endl;
        mPelletLogical->SetMaterial(NSMaterial::Get()->GetGe());
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();}
    else if (value=="Water"){
        G4cout << "Target Material set to Water" << G4endl; 
        mPelletLogical->SetMaterial(NSMaterial::Get()->GetWater());
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();}
    else{
        G4cout << "No know source Material, please change" << G4endl;}

}



