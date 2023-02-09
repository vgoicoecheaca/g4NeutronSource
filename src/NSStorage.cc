#include <iostream>
#include <sstream>
#include <fstream>

#include "TMath.h"
#include "TComplex.h"
#include "TComplex.h"
#include <complex>

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "NSStorage.hh"

using namespace std;

NSStorage* NSStorage::me = 0;

// singleton
NSStorage::NSStorage(){

   fNumberOfActiveLayers = 4;
   fAmSkinRadius         = 4*mm;
   fAmSkinThick     	 = 2*um;
   fPelletThick     	 = 1*mm;
   fAuSkinThick     	 = 2*um;
   fFoamThick       	 = 2*mm;   
   fAmCenter             = 0*mm;
   fGenerateInAm241      = 1;
   
}

NSStorage* NSStorage::Get() {
  if (!me)
    me = new NSStorage();

  return me;
}
