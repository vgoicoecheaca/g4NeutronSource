
#include "NSDetectorConstructionMessenger.hh"
#include "NSDetectorConstruction.hh"
#include "NSStorage.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4RunManager.hh"
class NSDetectorConstructionMessenger;

NSDetectorConstructionMessenger::NSDetectorConstructionMessenger(NSDetectorConstruction *det)
{
 fDetectorConstruction = det;
 fDirectory      = new G4UIdirectory("/ns/detector/");
 fDirectory      ->SetGuidance("Control the detector geometry");

 fNActiveLayers = new G4UIcmdWithAnInteger("/ns/detector/numberOfActiveLayers",this);

 fPelletThickCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/pelletThickness",this);
 fPelletThickCmd ->SetUnitCandidates("cm mm um nm");

 fAuLeafThickCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/AuLeafThickness",this);
 fAuLeafThickCmd->SetUnitCandidates("cm mm um nm");

 fAmThickCmd     = new G4UIcmdWithADoubleAndUnit("/ns/detector/AmThickness", this);
 fAmThickCmd      ->SetUnitCandidates("cm mm um nm");

 fFoamThickCmd   = new G4UIcmdWithADoubleAndUnit("/ns/detector/foamThickness",this);
 fFoamThickCmd   ->SetUnitCandidates("cm mm um nm");

}

NSDetectorConstructionMessenger::~NSDetectorConstructionMessenger()
{
    delete fNActiveLayers;
    delete fPelletThickCmd;
    delete fAuLeafThickCmd;
    delete fAmThickCmd;
    delete fFoamThickCmd;
}

void NSDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command==fNActiveLayers){
        NSStorage::Get()->SetNumberOfActiveLayers(fNActiveLayers->ConvertToInt(newValue));
	cout << "New value of Active Layers set " << fNActiveLayers->ConvertToInt(newValue)<< endl;
    }
    else if (command==fPelletThickCmd){
        NSStorage::Get()->SetPelletThickness(fPelletThickCmd->ConvertToDimensionedDouble(newValue));
	cout << "New value of Pellet thickness set [mm] " << fPelletThickCmd->ConvertToDimensionedDouble(newValue)/CLHEP::mm << endl;
    }
    else if (command==fAuLeafThickCmd){
        NSStorage::Get()->SetAuLeafThickness(fAuLeafThickCmd->ConvertToDimensionedDouble(newValue));
	cout << "New value of Au moderator thickness set [mm] " << fAuLeafThickCmd->ConvertToDimensionedDouble(newValue)/CLHEP::mm << endl;
    }
    else if (command==fAmThickCmd){
        NSStorage::Get()->SetAmThickness(fAmThickCmd->ConvertToDimensionedDouble(newValue));
	cout << "New value of Am241 thickness set [mm] " << fAmThickCmd->ConvertToDimensionedDouble(newValue)/CLHEP::mm<< endl;
    }
    else if (command==fFoamThickCmd){
        NSStorage::Get()->SetFoamThickness(fFoamThickCmd->ConvertToDimensionedDouble(newValue));
	cout << "New value of Foam thickness set [mm] " << fFoamThickCmd->ConvertToDimensionedDouble(newValue)/CLHEP::mm << endl;

    }
}
