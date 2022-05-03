#include <string>
#include <sstream>
#include <unistd.h>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include "NSDetectorConstruction.hh"
#include "PhysicsList.hh"
#include "NSPrimaryGeneratorAction.hh"
#include "NSAnalysisManager.hh"
#include "NSStackingAction.hh"
#include "NSRunAction.hh"
#include "NSEventAction.hh"
#include "NSActionInitialization.hh"

void usage();

int
main(int argc, char **argv)
{
	// switches
    int iNbEventsToSimulate = 0;	
	std::string hDataFilename;
    hDataFilename = "outfile.root";

	// parse switches
    G4String macrofile = "";
    G4UIExecutive* ui  = nullptr;
    if ( argc == 1 )
    {
      ui = new G4UIExecutive(argc, argv);
    }
    else
    {
      macrofile = argv[1];
      G4cout << " Reading G4 commands from macro file " << macrofile << " \n";
    }
	// create the run manager
	G4RunManager *pRunManager = new G4RunManager;

	// set user-defined initialization classes
	pRunManager->SetUserInitialization(new NSDetectorConstruction);
	pRunManager->SetUserInitialization(new PhysicsList);

	G4VisManager* pVisManager = new G4VisExecutive;
	pVisManager->Initialize();

	// create the primary generator action
	NSPrimaryGeneratorAction *pPrimaryGeneratorAction = new NSPrimaryGeneratorAction();

	// create an analysis manager object
	NSAnalysisManager *pAnalysisManager = new NSAnalysisManager(pPrimaryGeneratorAction);
	if(!hDataFilename.empty())
		pAnalysisManager->SetDataFilename(hDataFilename);
	if(iNbEventsToSimulate)
		pAnalysisManager->SetNbEventsToSimulate(iNbEventsToSimulate);

	// set user-defined action classes
	pRunManager->SetUserAction(pPrimaryGeneratorAction);
	pRunManager->SetUserAction(new NSStackingAction(pAnalysisManager));
	pRunManager->SetUserAction(new NSRunAction(pAnalysisManager));
	pRunManager->SetUserAction(new NSEventAction(pAnalysisManager));

	pRunManager->Initialize();

	G4UImanager* pUImanager = G4UImanager::GetUIpointer();

	if ( !ui )
    {
      // batch mode - just executes commands in the maro file
      G4String command  = "/control/execute ";
	  G4cout << "Executing the macro file  " << macrofile << G4endl;
      pUImanager->ApplyCommand( command + macrofile );
	  G4cout << "Done with the macro file" << G4endl;
    }
    else
    {
      // interactive mode: create ui-session       
      // 
      pUImanager->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      // Here we are after the ui-session termination!
      delete ui;
    }
	delete pVisManager; 
	delete pRunManager;
}

void
usage()
{
	exit(0);
}

