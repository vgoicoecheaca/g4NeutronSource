
#include "NSDetectorConstructionMessenger.hh"
#include "NSDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


NSDetectorConstructionMessenger::NSDetectorConstructionMessenger(NSDetectorConstruction *det)
{
 pDetectorConstruction = det;
 pDirectory = new G4UIdirectory("/ns/detector/");
 pDirectory->SetGuidance("Control the detector geometry");
 
 //same for the al disk
 pPlaceAlDiskCmd = new G4UIcmdWithABool("/ns/detector/alDisk",this);

 //set the disk or not  
 pPlaceAcDiskCmd = new G4UIcmdWithABool("/ns/detector/acDisk",this);

 // source window height and diameter, in the Al disk
 pAcDiskDCmd     = new G4UIcmdWithADoubleAndUnit("/ns/detector/window_D",this);
 pAcDiskDCmd->SetUnitCandidates("cm mm");
 pAcDiskHCmd     = new G4UIcmdWithADoubleAndUnit("/ns/detector/window_H",this);
 pAcDiskHCmd->SetUnitCandidates("cm mm");

 // type of source for the material 
 pSourceTypeCmd = new G4UIcmdWithAString("/ns/detector/source", this);
 pSourceTypeCmd->SetCandidates("AmBe AmC");
 
 // pellet thickness
 pPelletHCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/pelletH", this);
 pPelletHCmd->SetUnitCandidates("mm");
 pPelletHCmd->SetParameterName("H", true);
 pPelletHCmd->SetRange("H < 10 && H>0.1");

 //pellet placement
 pPlacePelletCmd = new G4UIcmdWithABool("/ns/detector/pellet",this);

 // foam placement
 pPlaceFoamCmd   = new G4UIcmdWithABool("/ns/detector/foam",this);

 // source position 
 pSourcePosCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/sourcePos", this);
 pSourcePosCmd->SetUnitCandidates("mm cm m"); 

 // target material
 pTargetMatCmd = new G4UIcmdWithAString("/ns/detector/targetMat", this);

 pSourceCaseHCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/caseH", this);
 pSourceCaseHCmd->SetUnitCandidates("mm cm"); 
 pSourceCaseDCmd = new G4UIcmdWithADoubleAndUnit("/ns/detector/caseD", this);
 pSourceCaseDCmd->SetUnitCandidates("mm cm"); 
}

NSDetectorConstructionMessenger::~NSDetectorConstructionMessenger()
{
    delete pDirectory;
    delete pPlaceAcDiskCmd;
    delete pPlaceAlDiskCmd;
    delete pAcDiskDCmd;
    delete pAcDiskHCmd;
    delete pSourceTypeCmd;
    delete pSourcePosCmd;
    delete pPelletHCmd;
    delete pPlacePelletCmd;
    delete pPlaceFoamCmd;
    delete pTargetMatCmd;
    delete pSourceCaseHCmd;
    delete pSourceCaseDCmd;
}

void NSDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command==pPlaceAcDiskCmd){
        pDetectorConstruction->PlaceAcDisk(pPlaceAcDiskCmd->GetNewBoolValue(newValue));
    }
    
    else if(command==pAcDiskHCmd){
        pDetectorConstruction->SetWindowH(pAcDiskHCmd->GetNewDoubleValue(newValue));
    }
   
    else if(command==pAcDiskDCmd){
        pDetectorConstruction->SetWindowD(pAcDiskDCmd->GetNewDoubleValue(newValue));
    }
  
    else if(command==pPlaceAlDiskCmd){
        pDetectorConstruction->PlaceAlDisk(pPlaceAlDiskCmd->GetNewBoolValue(newValue));
    }
   
    else if(command==pSourceTypeCmd){
        pDetectorConstruction->SetSourceMaterial(newValue);
    }

    else if(command==pSourcePosCmd){
        pDetectorConstruction->SetSourcePos(pSourcePosCmd->GetNewDoubleValue(newValue));
    }

    else if(command==pPelletHCmd){
        pDetectorConstruction->SetPelletH(pPelletHCmd->GetNewDoubleValue(newValue));
    }
   
    else if(command==pPlacePelletCmd){
        pDetectorConstruction->PlacePellet(pPlacePelletCmd->GetNewBoolValue(newValue));
    }
  
    else if(command==pPlaceFoamCmd){
        pDetectorConstruction->PlaceFoam(pPlaceFoamCmd->GetNewBoolValue(newValue));
    }

    else if(command ==pTargetMatCmd ){
        pDetectorConstruction->SetTargetMaterial(newValue);
    }

    else if (command==pSourceCaseDCmd){
        pDetectorConstruction->SetCaseD(pSourceCaseDCmd->GetNewDoubleValue(newValue));
    }

    else if (command==pSourceCaseHCmd){
        pDetectorConstruction->SetCaseH(pSourceCaseHCmd->GetNewDoubleValue(newValue));
    }
}
