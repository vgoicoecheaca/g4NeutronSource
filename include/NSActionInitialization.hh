
#ifndef NSACTIONINITIALIZATION_HH
#define NSACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

class NSActionInitialization : public G4VUserActionInitialization
{
  public:  // Method declarations
    
    // Constructor & Destructor
    NSActionInitialization();
    virtual ~NSActionInitialization(); 

    // (Pure) Virtual method to be implemented by the user to instantiate 
    // User Action class objects
    virtual void Build() const;

    // Virtual method to be implemented by the user to instantiate User RunAction
    // class object to be used by G4MTRunManager. THIS METHOD IS NOT INVOKED IN
    // THE SEQUENTIAL MODE! The user should not use this method to instantiate
    // user action classes rather than user RunAction.
    virtual void BuildForMaster() const;
};

#endif
