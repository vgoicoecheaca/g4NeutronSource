#ifndef NSDetectorConstructionMessenger_h
#define NSDetectorConstructionMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
using namespace std;

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
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    NSDetectorConstruction*      fDetectorConstruction;

    G4UIdirectory* 		 fDirectory;

    G4UIcmdWithADoubleAndUnit*   fPelletThickCmd;
    G4UIcmdWithADoubleAndUnit*   fAuLeafThickCmd;
    G4UIcmdWithADoubleAndUnit*   fAmThickCmd;
    G4UIcmdWithADoubleAndUnit*   fFoamThickCmd;
    G4UIcmdWithAnInteger*        fNActiveLayers;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

