
#include "NSActionInitialization.hh"
#include "NSPrimaryGeneratorAction.hh"
#include "NSDetectorConstruction.hh"

#include "NSPrimaryGeneratorAction.hh"

NSActionInitialization::NSActionInitialization()
  : G4VUserActionInitialization()
{
}
    

NSActionInitialization::~NSActionInitialization()
{
}


void NSActionInitialization::BuildForMaster() const
{
}


void NSActionInitialization::Build() const
{

  // Set UserPrimaryGeneratorAction
  NSPrimaryGeneratorAction* primaryAction = new NSPrimaryGeneratorAction();
  SetUserAction(primaryAction);
  
}  
