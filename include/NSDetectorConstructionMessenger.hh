#ifndef NSDetectorConstructionMessenger_h
#define NSDetectorConstructionMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

class NSDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NSDetectorConstructionMessenger: public G4UImessenger
{
  public:
  
    NSDetectorConstructionMessenger(NSDetectorConstruction* );
   ~NSDetectorConstructionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    NSDetectorConstruction*      pDetectorConstruction;

    G4UIdirectory *pDirectory;
    G4UIcmdWithADoubleAndUnit* pAcDiskDCmd;
    G4UIcmdWithADoubleAndUnit* pAcDiskHCmd;
    G4UIcmdWithAString* pSourceTypeCmd;
    G4UIcmdWithADoubleAndUnit* pSourcePosCmd;
    G4UIcmdWithABool* pPlaceAcDiskCmd;
    G4UIcmdWithABool* pPlaceAlDiskCmd;
    G4UIcmdWithADoubleAndUnit* pPelletHCmd; 
    G4UIcmdWithABool* pPlacePelletCmd;
    G4UIcmdWithABool* pPlaceFoamCmd;
    G4UIcmdWithAString* pTargetMatCmd;
    G4UIcmdWithADoubleAndUnit* pSourceCaseHCmd; 
    G4UIcmdWithADoubleAndUnit* pSourceCaseDCmd; 

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

