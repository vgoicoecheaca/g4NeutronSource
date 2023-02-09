#include <G4Geantino.hh>
#include <G4IonTable.hh>
#include <G4ThreeVector.hh>
#include <G4ParticleTable.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithABool.hh>
#include <G4Tokenizer.hh>
#include <G4ios.hh>
#include <fstream>
#include <iomanip>

#include "NSParticleSource.hh"

#include "NSParticleSourceMessenger.hh"

NSParticleSourceMessenger::NSParticleSourceMessenger(NSParticleSource *pParticleSource):
	mpParticleSource(pParticleSource), mbShootIon(false)
{
	mpParticleTable = G4ParticleTable::GetParticleTable();

	// create directory
	mpDirectory = new G4UIdirectory("/ns/gun/");
	mpDirectory->SetGuidance("Particle Source control commands.");

	// list available particles
	mpListCmd = new G4UIcmdWithoutParameter("/ns/gun/List", this);
	mpListCmd->SetGuidance("List available particles.");
	mpListCmd->SetGuidance(" Invoke G4ParticleTable.");

	// set particle  
	mpParticleCmd = new G4UIcmdWithAString("/ns/gun/particle", this);
	mpParticleCmd->SetGuidance("Set particle to be generated.");
	mpParticleCmd->SetGuidance(" (geantino is default)");
	mpParticleCmd->SetGuidance(" (ion can be specified for shooting ions)");
	mpParticleCmd->SetParameterName("particleName", true);
	mpParticleCmd->SetDefaultValue("geantino");
	G4String candidateList;
	G4int nPtcl = mpParticleTable->entries();

	for(G4int i = 0; i < nPtcl; i++)
	{
		candidateList += mpParticleTable->GetParticleName(i);
		candidateList += " ";
	}
	candidateList += "ion ";
	mpParticleCmd->SetCandidates(candidateList);

	// particle direction
	mpDirectionCmd = new G4UIcmdWith3Vector("/ns/gun/direction", this);
	mpDirectionCmd->SetGuidance("Set momentum direction.");
	mpDirectionCmd->SetGuidance("Direction needs not to be a unit vector.");
	mpDirectionCmd->SetParameterName("Px", "Py", "Pz", true, true);
	mpDirectionCmd->SetRange("Px != 0 || Py != 0 || Pz != 0");

	// particle energy
	mpEnergyCmd = new G4UIcmdWithADoubleAndUnit("/ns/gun/energy", this);
	mpEnergyCmd->SetGuidance("Set kinetic energy.");
	mpEnergyCmd->SetParameterName("Energy", true, true);
	mpEnergyCmd->SetDefaultUnit("GeV");
	//mpEnergyCmd->SetUnitCategory("Energy");
	//mpEnergyCmd->SetUnitCandidates("eV keV MeV GeV TeV");

	mpPositionCmd = new G4UIcmdWith3VectorAndUnit("/ns/gun/position", this);
	mpPositionCmd->SetGuidance("Set starting position of the particle.");
	mpPositionCmd->SetParameterName("X", "Y", "Z", true, true);
	mpPositionCmd->SetDefaultUnit("cm");
	//mpPositionCmd->SetUnitCategory("Length");
	//mpPositionCmd->SetUnitCandidates("microm mm cm m km");

	// ion 
	mpIonCmd = new G4UIcommand("/ns/gun/ion", this);
	mpIonCmd->SetGuidance("Set properties of ion to be generated.");
	mpIonCmd->SetGuidance("[usage] /gun/ion Z A Q E");
	mpIonCmd->SetGuidance("        Z:(int) AtomicNumber");
	mpIonCmd->SetGuidance("        A:(int) AtomicMass");
	mpIonCmd->SetGuidance("        Q:(int) Charge of Ion (in unit of e)");
	mpIonCmd->SetGuidance("        E:(double) Excitation energy (in keV)");

	G4UIparameter *param;

	param = new G4UIparameter("Z", 'i', false);
	param->SetDefaultValue("1");
	mpIonCmd->SetParameter(param);
	param = new G4UIparameter("A", 'i', false);
	param->SetDefaultValue("1");
	mpIonCmd->SetParameter(param);
	param = new G4UIparameter("Q", 'i', true);
	param->SetDefaultValue("0");
	mpIonCmd->SetParameter(param);
	param = new G4UIparameter("E", 'd', true);
	param->SetDefaultValue("0.0");
	mpIonCmd->SetParameter(param);

	// source distribution type
	mpTypeCmd = new G4UIcmdWithAString("/ns/gun/type", this);
	mpTypeCmd->SetGuidance("Sets source distribution type.");
	mpTypeCmd->SetGuidance("Point,Plane,Surface,Volume");
	mpTypeCmd->SetParameterName("DisType", true, true);
	mpTypeCmd->SetDefaultValue("Point");
	mpTypeCmd->SetCandidates("Point Plane Volume Surface");

	// source shape
	mpShapeCmd = new G4UIcmdWithAString("/ns/gun/shape", this);
	mpShapeCmd->SetGuidance("Sets source shape type.");
	mpShapeCmd->SetParameterName("Shape", true, true);
	mpShapeCmd->SetDefaultValue("NULL");
	mpShapeCmd->SetCandidates("Sphere Disk Cylinder");

	// center coordinates
	mpCenterCmd = new G4UIcmdWith3VectorAndUnit("/ns/gun/center", this);
	mpCenterCmd->SetGuidance("Set center coordinates of source.");
	mpCenterCmd->SetParameterName("X", "Y", "Z", true, true);
	mpCenterCmd->SetDefaultUnit("cm");
	mpCenterCmd->SetUnitCandidates("nm mum mm cm m km");

	// half height of source
	mpHalfzCmd = new G4UIcmdWithADoubleAndUnit("/ns/gun/halfz", this);
	mpHalfzCmd->SetGuidance("Set z half length of source.");
	mpHalfzCmd->SetParameterName("Halfz", true, true);
	mpHalfzCmd->SetDefaultUnit("cm");
	mpHalfzCmd->SetUnitCandidates("nm mum mm cm m km");

	// radius of source  
	mpRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ns/gun/radius", this);
	mpRadiusCmd->SetGuidance("Set radius of source.");
	mpRadiusCmd->SetParameterName("Radius", true, true);
	mpRadiusCmd->SetDefaultUnit("cm");
	mpRadiusCmd->SetUnitCandidates("nm mum mm cm m km");

	// confine to volume(s)
	mpConfineCmd = new G4UIcmdWithAString("/ns/gun/confine", this);
	mpConfineCmd->SetGuidance("Confine source to volume(s) (NULL to unset).");
	mpConfineCmd->SetGuidance("usage: confine VolName1 VolName2 ...");
	mpConfineCmd->SetParameterName("VolName", true, true);
	mpConfineCmd->SetDefaultValue("NULL");

	// angular distribution
	mpAngTypeCmd = new G4UIcmdWithAString("/ns/gun/angtype", this);
	mpAngTypeCmd->SetGuidance("Sets angular source distribution type");
	mpAngTypeCmd->SetGuidance("Possible variables are: iso direction");
	mpAngTypeCmd->SetParameterName("AngDis", true, true);
	mpAngTypeCmd->SetDefaultValue("iso");
	mpAngTypeCmd->SetCandidates("iso direction");

	// energy distribution
	mpEnergyTypeCmd = new G4UIcmdWithAString("/ns/gun/energytype", this);
	mpEnergyTypeCmd->SetGuidance("Sets energy distribution type");
	mpEnergyTypeCmd->SetGuidance("Possible variables are: Mono, Kr83m");
	mpEnergyTypeCmd->SetParameterName("EnergyDis", true, true);
	mpEnergyTypeCmd->SetDefaultValue("Mono");
	mpEnergyTypeCmd->SetCandidates("Mono Kr83m");

    // number of particles to be generated
    mpNumberOfParticlesToBeGeneratedCmd
        = new G4UIcmdWithAnInteger("/ns/gun/numberofparticles", this);
    mpNumberOfParticlesToBeGeneratedCmd->SetGuidance("Number of particles generated in one event");
    mpNumberOfParticlesToBeGeneratedCmd->SetParameterName("NumParticles", true, true);
    mpNumberOfParticlesToBeGeneratedCmd->SetDefaultValue(1);

	// verbosity
	mpVerbosityCmd = new G4UIcmdWithAnInteger("/ns/gun/verbose", this);
	mpVerbosityCmd->SetGuidance("Set Verbose level for gun");
	mpVerbosityCmd->SetGuidance(" 0 : Silent");
	mpVerbosityCmd->SetGuidance(" 1 : Limited information");
	mpVerbosityCmd->SetGuidance(" 2 : Detailed information");
	mpVerbosityCmd->SetParameterName("level", false);
	mpVerbosityCmd->SetRange("level>=0 && level <=2");

}

NSParticleSourceMessenger::~NSParticleSourceMessenger()
{
	delete mpTypeCmd;
	delete mpShapeCmd;
	delete mpCenterCmd;
	delete mpHalfzCmd;
	delete mpRadiusCmd;
	delete mpConfineCmd;
	delete mpAngTypeCmd;
	delete mpEnergyTypeCmd;
	delete mpVerbosityCmd;
	delete mpIonCmd;
	delete mpParticleCmd;
	delete mpPositionCmd;
	delete mpDirectionCmd;
	delete mpEnergyCmd;
	delete mpListCmd;

	delete mpDirectory;
}

void
NSParticleSourceMessenger::SetNewValue(G4UIcommand * command, G4String newValues)
{
	if(command == mpTypeCmd)
		mpParticleSource->SetPosDisType(newValues);

	else if(command == mpShapeCmd)
		mpParticleSource->SetPosDisShape(newValues);

	else if(command == mpCenterCmd)
		mpParticleSource->SetCenterCoords(mpCenterCmd->GetNew3VectorValue(newValues));

	else if(command == mpHalfzCmd)
		mpParticleSource->SetHalfZ(mpHalfzCmd->GetNewDoubleValue(newValues));

	else if(command == mpRadiusCmd)
		mpParticleSource->SetRadius(mpRadiusCmd->GetNewDoubleValue(newValues));

	else if(command == mpAngTypeCmd)
		mpParticleSource->SetAngDistType(newValues);

	else if(command == mpConfineCmd)
		mpParticleSource->ConfineSourceToVolume(newValues);

	else if(command == mpEnergyTypeCmd)
		mpParticleSource->SetEnergyDisType(newValues);

	else if(command == mpVerbosityCmd)
		mpParticleSource->SetVerbosity(mpVerbosityCmd->GetNewIntValue(newValues));

	else if(command == mpParticleCmd)
	{
		if(newValues == "ion")
		{
			mbShootIon = true;
		}
		else
		{
			mbShootIon = false;
			G4ParticleDefinition * pd = mpParticleTable->FindParticle(newValues);

			if(pd != NULL)
			{
				mpParticleSource->SetParticleDefinition(pd);
			}
		}
	}

	else if(command == mpIonCmd)
	{
		if(mbShootIon)
		{
			G4Tokenizer next(newValues);

			// check argument
			miAtomicNumber = StoI(next());
			miAtomicMass = StoI(next());
			G4String sQ = next();

			if(sQ.empty())
			{
				miIonCharge = miAtomicNumber;
			}
			else
			{
				miIonCharge = StoI(sQ);
				sQ = next();
				if(sQ.empty())
				{
					mdIonExciteEnergy = 0.0;
				}
				else
				{
					mdIonExciteEnergy = StoD(sQ) * CLHEP::keV;
				}
			}

			G4ParticleDefinition *ion;

			//ion = mpParticleTable->G4IonTable::GetIon(miAtomicNumber, miAtomicMass, mdIonExciteEnergy);
			ion = G4IonTable::GetIonTable()->GetIon(miAtomicNumber, miAtomicMass, mdIonExciteEnergy);
			if(ion == 0)
			{
				G4cout << "Ion with Z=" << miAtomicNumber;
				G4cout << " A=" << miAtomicMass << "is not be defined" << G4endl;
			}
			else
			{
				mpParticleSource->SetParticleDefinition(ion);
				mpParticleSource->SetParticleCharge(miIonCharge *CLHEP::eplus);
			}
		}
		else
		{
			G4cout << "Set /ns/gun/particle to ion before using /ns/gun/ion command";
			G4cout << G4endl;
		}
	}

	else if(command == mpListCmd)
		mpParticleTable->DumpTable();

	else if(command == mpDirectionCmd)
	{
		mpParticleSource->SetAngDistType("direction");
		mpParticleSource->SetParticleMomentumDirection(mpDirectionCmd->GetNew3VectorValue(newValues));
	}

	else if(command == mpEnergyCmd)
	{
		mpParticleSource->SetEnergyDisType("Mono");
		mpParticleSource->SetMonoEnergy(mpEnergyCmd->GetNewDoubleValue(newValues));
	}

	else if(command == mpPositionCmd)
	{
		mpParticleSource->SetPosDisType("Point");
		mpParticleSource->SetCenterCoords(mpPositionCmd->GetNew3VectorValue(newValues));
	}
	else
		G4cout << "Error entering command" << G4endl;
}

