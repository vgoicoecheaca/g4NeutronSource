#include "NSMaterial.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh" 

#include <fstream>

using namespace std;

NSMaterial* NSMaterial::me =0;

NSMaterial::NSMaterial() 
{
  DefineMaterials();
}

NSMaterial::~NSMaterial(){}

NSMaterial* NSMaterial::Get() {
  if (!me){
    me = new NSMaterial();}
  return me;
}

void NSMaterial::DefineMaterials(){

    G4String name,
             symbol;
    G4int    n, 
             numisotopes,
             iz,
             numberOfAtoms,
             ncomponents;
    G4double density, 
             fractionmass; 

  G4NistManager* nistMGR = G4NistManager::Instance();

  // define the needed isotopes
  G4Isotope* isC  = new G4Isotope(name="C13",iz=6,n=13);  
  G4Isotope* isAm = new G4Isotope(name="Am241",iz=95,n=241);

  //define the needed elements 
  G4Element* elBe  = nistMGR->FindOrBuildElement("Be");
  G4Element* elC  = new G4Element(name="Carbon-13", symbol = "C",numisotopes=1);
  elC->AddIsotope(isC,100.*perCent); 
  G4Element* elO  = nistMGR->FindOrBuildElement("O");
  G4Element *elAl = nistMGR->FindOrBuildElement("Al");
  G4Element *elSi = nistMGR->FindOrBuildElement("Si");
  G4Element *elCu = nistMGR->FindOrBuildElement("Cu");
  G4Element *elFe = nistMGR->FindOrBuildElement("Fe");
  G4Element* elMn = nistMGR->FindOrBuildElement("Mn");
  G4Element *elTi = nistMGR->FindOrBuildElement("Ti");
  G4Element* elCr = nistMGR->FindOrBuildElement("Cr");
  G4Element *elMg = nistMGR->FindOrBuildElement("Mg");
  G4Element *elZn = nistMGR->FindOrBuildElement("Zn");
  G4Element* elAm = new G4Element(name="Am-241", symbol = "Am",numisotopes=1);
  elAm->AddIsotope(isAm,100.*perCent);

  // Define the needed materials
  mBe = nistMGR->FindOrBuildMaterial("G4_Be");
  mC    = new G4Material(name ="Carbon-13" ,  density=2.26*g/cm3,  ncomponents=1); 
  mC->AddElement(elC,fractionmass = 1);  
  mBeO = nistMGR->FindOrBuildMaterial("G4_BERYLLIUM_OXIDE");
  mAl = new G4Material("Aluminum_6061",2.70*g/cm3,9);
  mAl->AddElement(elAl,0.9668);
  mAl->AddElement(elSi,0.0060);
  mAl->AddElement(elFe,0.0070);
  mAl->AddElement(elCu,0.0028);
  mAl->AddElement(elMn,0.0015);
  mAl->AddElement(elMg,0.0100);
  mAl->AddElement(elCr,0.0019);
  mAl->AddElement(elZn,0.0025);
  mAl->AddElement(elTi,0.0015);
  mCu   = nistMGR->FindOrBuildMaterial("G4_Cu");
  mGe   = nistMGR->FindOrBuildMaterial("G4_Ge");
  mAu   = nistMGR->FindOrBuildMaterial("G4_Au");
  mPb   = nistMGR->FindOrBuildMaterial("G4_Pb"); 
  mAm   = new G4Material(name ="Americium",density=13.67*g/cm3, ncomponents=1); 
  mAm->AddElement(elAm,fractionmass = 1);
  mAmO2 = new G4Material(name ="AmO2",density=11.68*g/cm3, ncomponents=2); 
  mAmO2->AddElement(elAm,numberOfAtoms=1);
  mAmO2->AddElement(elO, numberOfAtoms=2);
  mAmO2Be13 = new G4Material(name = "AmO2Be13", density = 3.69*g/cm3, ncomponents=3);
  mAmO2Be13->AddElement(elAm,numberOfAtoms=1);
  mAmO2Be13->AddElement(elO, numberOfAtoms=2);
  mAmO2Be13->AddElement(elBe, numberOfAtoms=13); 
  mFoam = nistMGR->FindOrBuildMaterial("G4_POLYETHYLENE"); 
  mAir  = nistMGR->FindOrBuildMaterial("G4_AIR"); 
  mAcrylic = new G4Material(name="Acrylic",density=1.19*g/cm3, ncomponents=3, kStateSolid);
  mAcrylic->AddElement(nistMGR->FindOrBuildElement("C"), 5);
  mAcrylic->AddElement(nistMGR->FindOrBuildElement("O"), 2);
  mAcrylic->AddElement(nistMGR->FindOrBuildElement("H"), 8);
  mSteel = nistMGR->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  mWater = nistMGR->FindOrBuildMaterial("G4_WATER");
}


