#ifndef NSDMATERIAL_HH
#define NSDMATERIAL_HH

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

class NSMaterial  {

  private:
    NSMaterial();
    static NSMaterial *me;

  public:
    static NSMaterial* Get();
   ~NSMaterial();

    G4Material*  GetMaterial(G4String );
    void DefineMaterials();

    G4Material*  GetBe()                                { return mBe	   ;}
    G4Material*  GetC()                                 { return mC		   ;}
    G4Material*  GetBeO()                               { return mBeO    ;}
    G4Material*  GetAl()                                { return mAl		 ;}
    G4Material*  GetCu()                                { return mCu		 ;}
    G4Material*  GetGe()                                { return mGe		 ;}
    G4Material*  GetAu()                                { return mAu	   ;}
    G4Material*  GetPb()                                { return mPb	   ;}
    G4Material*  GetAm()                                { return mAm	   ;}
    G4Material*  GetAmO2()                              { return mAmO2    ;}
    G4Material*  GetAmO2Be13()                          { return mAmO2Be13;}
    G4Material*  GetFoam()                              { return mFoam	 ;}
    G4Material*  GetAir()                               { return mAir    ;}
    G4Material*  GetAcrylic()                           { return mAcrylic;}
    G4Material*  GetSteel()                             { return mSteel;}
    G4Material*  GetWater()                             { return mWater;}
  private:

    G4Material*                mBe; 
    G4Material*                mC; 
    G4Material*                mBeO;
    G4Material*                mAl; 
    G4Material*                mCu; 
    G4Material*                mGe; 
    G4Material*                mAu; 
    G4Material*                mPb; 
    G4Material*                mAm; 
    G4Material*                mAmO2;
    G4Material*                mAmO2Be13;
    G4Material*                mFoam;
    G4Material*                mAir; 
    G4Material*                mAcrylic;
    G4Material*                mSteel;
    G4Material*                mWater;
};
#endif