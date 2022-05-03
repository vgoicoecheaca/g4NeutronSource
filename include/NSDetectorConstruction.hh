#ifndef NSDETECTORCONTRUCTION_HH
#define NSDETECTORCONTRUCTION_HH 1 

#include "G4VUserDetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class NSDetectorConstructionMessenger;

class NSDetectorConstruction : public G4VUserDetectorConstruction
{

  public:  

    NSDetectorConstruction();           // Constructor
    ~NSDetectorConstruction();  // Destructor

    virtual G4VPhysicalVolume* Construct();

    void PlaceAcDisk(G4bool value);   
    void SetWindowD(G4double value);
    void SetWindowH(G4double value);
    void PlaceAlDisk(G4bool value);   
    void SetSourceMaterial(G4String value);
    void SetSourcePos(G4double value);
    void SetPelletH(G4double value);
    void SetCaseH(G4double value);
    void SetCaseD(G4double value);
    void PlacePellet(G4bool value);
    void PlaceFoam(G4bool value);
    void SetTargetMaterial(G4String value);

    NSDetectorConstructionMessenger* pDetectorMessenger;  

  private:
    //names 
    G4String pelletName;

    //positions

    //dimensions
    G4double outerShieldX; 
    G4double outerShieldY; 
    G4double outerShieldZ; 
    G4double innerShieldX; 
    G4double innerShieldY; 
    G4double innerShieldZ; 
    G4double targetD     ; 
    G4double targetH     ; 
    G4double targetPhi   ; 
    G4double acDiskD     ; 
    G4double acDiskH     ; 
    G4double alDiskD     ; 
    G4double alDiskH     ; 
    G4double outerGapX   ; 
    G4double outerGapY   ; 
    G4double outerGapZ   ; 
    G4double innerGapX   ; 
    G4double innerGapY   ; 
    G4double innerGapZ   ; 
    G4double windowD     ; 
    G4double windowH     ; 
    G4double sourceGapZ  ; 
    G4double sourceGap0  ; 

    G4double caseD         ;
    G4double caseH         ;
    G4double holderD       ;
    G4double holderH       ;
    G4double holderGapD    ;
    G4double holderGapH    ;
    G4double sourceH       ;
    G4double moderatorH    ;
    G4double foamD         ;
    G4double foamH         ;
    G4double pelletH       ;
    G4double pelletD;
    G4ThreeVector sourcePos    ; 
    G4ThreeVector am241pos;
    G4ThreeVector sourceGapPos ; 
    G4ThreeVector sourceFilmPos; 
    G4ThreeVector modPosBel    ; 
    G4ThreeVector pelletPos    ; 
    G4ThreeVector modPosAb     ; 
    G4ThreeVector foamPos      ; 
    G4double am241_pos;

    //volumes and logicals

    G4LogicalVolume *    labLogical;
    G4VPhysicalVolume *  mLabPhysical;
    G4VPhysicalVolume *  mInnerShieldPhysical; 
    G4VPhysicalVolume *  mOuterShieldPhysical; 

    G4Tubs*              alDiskSolid ;    
    G4LogicalVolume*     alDiskLogic; 
    G4VPhysicalVolume *  mAlDiskPhysical;
  
    G4Tubs*              acDiskSolid;
    G4Tubs*              windowSolid;
    G4SubtractionSolid*  acDiskWindowSolid; 
    G4LogicalVolume*     acDiskLogic; 
    G4VPhysicalVolume *  mAcDiskPhysical;
   
    G4VPhysicalVolume *  mTargetPhysical;
    G4VPhysicalVolume *  mSourceCasePhysical;
    G4VPhysicalVolume *  mAmPhysical ;
    G4VPhysicalVolume *  mHolderPhysical ;

    G4Tubs * moderatorSolid; 
    G4LogicalVolume * moderatorLogical;
    G4VPhysicalVolume *  mModeratorPhysical_bel ;
    G4VPhysicalVolume *  mModeratorPhysical_ab ;

    G4Tubs* foamSolid;           
    G4LogicalVolume* foamLogical;
    G4VPhysicalVolume* mFoamPhysical ;

    G4Tubs* pelletSolid;
    G4LogicalVolume * mPelletLogical;
    G4VPhysicalVolume *  mPelletPhysical ;
    
    G4VPhysicalVolume* ConstructVolumes();     
};

#endif
