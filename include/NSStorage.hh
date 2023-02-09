#ifndef _NSStorage_HH
#define _NSStorage_HH 1

#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "TComplex.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class NSStorage  {
  private:
    NSStorage();
  public:
    static NSStorage* Get();

    virtual ~NSStorage() {}

  private:

    static NSStorage *me;

  public:

    inline void SetNumberOfActiveLayers(G4int val)                {fNumberOfActiveLayers = val;};
    inline G4int GetNumberOfActiveLayers()                        {return fNumberOfActiveLayers;};
  
    inline void SetFoamThickness(G4double val)     		  {fFoamThick = val;};
    inline G4double GetFoamThickness()             		  {return fFoamThick;};

    inline void SetPelletThickness(G4double val)     		  {fPelletThick = val;};   
    inline G4double GetPelletThickness()             		  {return fPelletThick;};

    inline void SetAuLeafThickness(G4double val)                  {fAuSkinThick = val;};
    inline G4double GetAuLeafThickness()             		  {return fAuSkinThick;};

    inline void SetAmThickness(G4double val)                      {fAmSkinThick = val;};
    inline G4double GetAmThickness()             		  {return fAmSkinThick;};

    inline void SetAmRadius(G4double val)     		          {fAmSkinRadius = val;};
    inline G4double GetAmRadius()             		          {return fAmSkinRadius;};

    inline void SetAmCenter(G4double val)                         {fAmCenter = val;};
    inline G4double  GetAmCenter()             		          {return fAmCenter;};

    inline void SetGenerateInAm241(G4bool val) 			  {fGenerateInAm241 = val;};
    inline G4bool GetGenerateInAm241()                            {return fGenerateInAm241;};

  private:

    G4int    	      fNumberOfActiveLayers;
    G4double 	      fFoamThick;
    G4double 	      fPelletThick;      
    G4double 	      fAuSkinThick;
    G4double 	      fAmSkinThick;
    G4double 	      fAmSkinRadius;
    G4double          fAmCenter;
    G4bool 	      fGenerateInAm241;

};

#endif
