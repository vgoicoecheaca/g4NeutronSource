#ifndef NSSourceConstructor_H
#define NSSourceConstructor_H

#include "G4LogicalVolume.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4VPhysicalVolume.hh"

#include "G4MaterialPropertiesTable.hh"


class	NSMaterial;

class   NSSourceConstructor  {

public:
  
  NSSourceConstructor(G4VPhysicalVolume*);
  ~NSSourceConstructor();
  
private:
  void                        DefineSurfaces();

  G4VPhysicalVolume*  fScreenPhys;
  G4VPhysicalVolume*  testPhys;
  G4VPhysicalVolume*  fMotherVolume;
  G4VPhysicalVolume*  fCasePhys;
  G4VPhysicalVolume*  fHolderPhys;
  G4VPhysicalVolume*  fAirPhys;
  G4VPhysicalVolume*  fFoamPhys;
};

#endif
