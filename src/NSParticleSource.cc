#include <G4PrimaryParticle.hh>
#include <G4Event.hh>
#include <G4TransportationManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4ParticleTable.hh>
#include "G4NuclideTable.hh"
#include <G4IonTable.hh>
#include <G4Ions.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <Randomize.hh>
#include "NSStorage.hh"

#include <sstream>
#include <cmath>

using std::stringstream;

#include "NSParticleSource.hh"

NSParticleSource::NSParticleSource()
{
	particleTable = G4ParticleTable::GetParticleTable();

	miNumberOfParticlesToBeGenerated = 1;
	mpParticleDefinition = 0;
	G4ThreeVector hZero(0., 0., 0.);

	mhParticleMomentumDirection = G4ParticleMomentum(1., 0., 0.);
	mdParticleEnergy = 1.0*CLHEP::MeV;
	mhParticlePosition = hZero;
	mdParticleTime = 0.0;
	mhParticlePolarization = hZero;
	mdParticleCharge = 0.0;

	mhSourcePosType = "Volume";
	mhShape = "NULL";
	mdHalfz = 0.;
	mdRadius = 0.;
	mhCenterCoords = hZero;
	mbConfine = false;
	mhVolumeNames.clear();

	mhAngDistType = "iso";
	mdMinTheta = 0.;
	mdMaxTheta = CLHEP::pi;
	mdMinPhi = 0.;
	mdMaxPhi = CLHEP::twopi;

	mhEnergyDisType = "Mono";
	mdMonoEnergy = 1*CLHEP::MeV;

	miVerbosityLevel = 0;

	mpMessenger = new NSParticleSourceMessenger(this);
	mpNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

NSParticleSource::~NSParticleSource()
{
	delete mpMessenger;
}

void
NSParticleSource::ConfineSourceToVolume(G4String hVolumeList)
{
	stringstream hStream;
	hStream.str(hVolumeList);
	G4String hVolumeName;

	// store all the volume names
	while(!hStream.eof())
	{
		hStream >> hVolumeName;
		cout << "hVolumeName "<< hVolumeName << endl;
		mhVolumeNames.insert(hVolumeName);
	}

	// checks if the selected volumes exist and store all volumes that match
	G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
	G4bool bFoundAll = true;

	set<G4String> hActualVolumeNames;
	for(set<G4String>::iterator pIt = mhVolumeNames.begin(); pIt != mhVolumeNames.end(); pIt++)
	{
		G4String hRequiredVolumeName = *pIt;
		G4bool bMatch = false;

		if(bMatch = (hRequiredVolumeName.rfind('*') != std::string::npos))
			hRequiredVolumeName = hRequiredVolumeName.strip(G4String::trailing, '*');

		G4bool bFoundOne = false;
		for(G4int iIndex = 0; iIndex < (G4int) PVStore->size(); iIndex++)
		{
			G4String hName = (*PVStore)[iIndex]->GetName();
			cout << "hName in NSparticleSource " << hName << endl;

			if((bMatch && (hName.substr(0, hRequiredVolumeName.size())) == hRequiredVolumeName) || hName == hRequiredVolumeName)
			{
				hActualVolumeNames.insert(hName);
				bFoundOne = true;
			}
		}

		bFoundAll = bFoundAll && bFoundOne;
	}

	if(bFoundAll)
	{
		mhVolumeNames = hActualVolumeNames;
		mbConfine = true;

		if(miVerbosityLevel >= 1)
			G4cout << "Source confined to volumes: " << hVolumeList << G4endl;

		if(miVerbosityLevel >= 2)
		{
			G4cout << "Volume list: " << G4endl;

			for(set<G4String>::iterator pIt = mhVolumeNames.begin(); pIt != mhVolumeNames.end(); pIt++)
				G4cout << *pIt << G4endl;
		}
	}
	else if(mhVolumeNames.empty())
		mbConfine = false;
	else
	{
		G4cout << " **** Error: One or more volumes do not exist **** " << G4endl;
		G4cout << " Ignoring confine condition" << G4endl;
		mhVolumeNames.clear();
		mbConfine = false;
	}
}

void
NSParticleSource::GeneratePointSource()
{
	// Generates Points given the point source.
	if(mhSourcePosType == "Point")
		mhParticlePosition = mhCenterCoords;
	else if(miVerbosityLevel >= 1)
		G4cout << "Error SourcePosType is not set to Point" << G4endl;
}

void
NSParticleSource::GeneratePointsInVolume()
{
	G4ThreeVector RandPos;
	G4double x = 0., y = 0., z = 0.;

	if(mhSourcePosType != "Volume" && miVerbosityLevel >= 1)
		G4cout << "Error SourcePosType not Volume" << G4endl;

	if(mhShape == "Sphere")
	{
		x = mdRadius * 2.;
		y = mdRadius * 2.;
		z = mdRadius * 2.;
		while(((x * x) + (y * y) + (z * z)) > (mdRadius * mdRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();

			x = (x * 2. * mdRadius) - mdRadius;
			y = (y * 2. * mdRadius) - mdRadius;
			z = (z * 2. * mdRadius) - mdRadius;
		}
	}

	else if(mhShape == "Cylinder")
	{
		x = mdRadius * 2.;
		y = mdRadius * 2.;
		while(((x * x) + (y * y)) > (mdRadius * mdRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();
			x = (x * 2. * mdRadius) - mdRadius;
			y = (y * 2. * mdRadius) - mdRadius;
			z = (z * 2. * mdHalfz) - mdHalfz;
		}

	}

	else
		G4cout << "Error: Volume Shape Does Not Exist" << G4endl;

	RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	mhParticlePosition = mhCenterCoords + RandPos;

}

void NSParticleSource::GeneratePointsInPlane()
{
	G4ThreeVector RandPos;
	G4double x = 0., y = 0., z = 0;

	if (mhShape=="Disk")
	{
		x = mdRadius * 2.;
		y = mdRadius * 2.;
      	while(((x * x) + (y * y)) > (mdRadius * mdRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			x = (x * 2. * mdRadius) - mdRadius;
			y = (y * 2. * mdRadius) - mdRadius;
		}
	}	
	else
		G4cout << "Error: Plane Shape Does Not Exist" << G4endl;
	
    RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	mhParticlePosition = mhCenterCoords + RandPos;	
}

void NSParticleSource::GeneratePointsInSurface()
{
	G4ThreeVector RandPos;
	G4double x = 0., y = 0., z = 0.;

	if (mhShape=="Cylinder")
	{
		x = mdRadius * 2.;
		y = mdRadius * 2.;
		while(((x * x) + (y * y)) == (mdRadius * mdRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();
			x = (x * 2. * mdRadius) - mdRadius;
			y = (y * 2. * mdRadius) - mdRadius;
			z = (z * 2. * mdHalfz) - mdHalfz;
		}
	}
	else
		G4cout << "Error: Surface Shape Does Not Exist" << G4endl;

	RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	mhParticlePosition = mhCenterCoords + RandPos;
}


G4bool
NSParticleSource::IsSourceConfined()
{
	// Method to check point is within the volume specified
	if(mbConfine == false)
		G4cout << "Error: Confine is false" << G4endl;
	G4ThreeVector null(0., 0., 0.);
	G4ThreeVector *ptr;

	ptr = &null;

	// Check mhParticlePosition is within a volume in our list
	G4VPhysicalVolume *theVolume;

	theVolume = mpNavigator->LocateGlobalPointAndSetup(mhParticlePosition, ptr, true);
	G4String theVolName = theVolume->GetName();

	set<G4String>::iterator pIt;
	if((pIt = mhVolumeNames.find(theVolName)) != mhVolumeNames.end())
	{
		if(miVerbosityLevel >= 1)
			G4cout << "Particle is in volume " << *pIt << G4endl;
		return (true);
	}
	else
		return (false);
}

void
NSParticleSource::GenerateIsotropicFlux()
{
	G4double rndm, rndm2;
	G4double px, py, pz;

	G4double sintheta, sinphi, costheta, cosphi;

	rndm = G4UniformRand();
	costheta = std::cos(mdMinTheta) - rndm * (std::cos(mdMinTheta) - std::cos(mdMaxTheta));
	sintheta = std::sqrt(1. - costheta * costheta);

	rndm2 = G4UniformRand();
	mdPhi = mdMinPhi + (mdMaxPhi - mdMinPhi) * rndm2;
	sinphi = std::sin(mdPhi);
	cosphi = std::cos(mdPhi);

	px = -sintheta * cosphi;
	py = -sintheta * sinphi;
	pz = -costheta;

	G4double ResMag = std::sqrt((px * px) + (py * py) + (pz * pz));

	px = px / ResMag;
	py = py / ResMag;
	pz = pz / ResMag;

	mhParticleMomentumDirection.setX(px);
	mhParticleMomentumDirection.setY(py);
	mhParticleMomentumDirection.setZ(pz);

	// mhParticleMomentumDirection now holds unit momentum vector.
	if(miVerbosityLevel >= 2)
		G4cout << "Generating isotropic vector: " <<
			mhParticleMomentumDirection << G4endl;
}

void NSParticleSource::GenerateInAm241(){

   	mhSourcePosType 	= "Volume";
        mhShape 		= "Cylinder";
	mbConfine 		= true;	
        mdHalfz 		= NSStorage::Get()->GetAmThickness()/2;
        mdRadius 		= NSStorage::Get()->GetAmRadius();
	G4int numberOfLayers    = NSStorage::Get()->GetNumberOfActiveLayers();	
	G4double amFirstCenter  = NSStorage::Get()->GetAmCenter();
	G4int randomIdx         = rand() % numberOfLayers;
	G4double amCenter       = amFirstCenter;
	if (randomIdx==0) mhCenterCoords = G4ThreeVector(0.,0.,amFirstCenter);
	else  if (randomIdx>=1) {
		for (int m=1;m<=1;++m) {
			amCenter = amCenter - (4*NSStorage::Get()->GetAuLeafThickness() + NSStorage::Get()->GetAmThickness() + NSStorage::Get()->GetPelletThickness());}
		mhCenterCoords = G4ThreeVector(0.,0.,amCenter);
		}

	// fix this, cheap
	//if      (randomIdx == 0) mhVolumeNames.insert("Am241Phys0");
	//else if (randomIdx == 1) mhVolumeNames.insert("Am241Phys1");
	//else if (randomIdx == 2) mhVolumeNames.insert("Am241Phys2");
        //else if (randomIdx == 3) mhVolumeNames.insert("Am241Phys3");
	//else if (randomIdx == 4) mhVolumeNames.insert("Am241Phys4");

}

void
NSParticleSource::GenerateMonoEnergetic()
{
	mdParticleEnergy = mdMonoEnergy;
}


void
NSParticleSource::SetParticleDefinition(G4ParticleDefinition * aParticleDefinition)
{
	mpParticleDefinition = aParticleDefinition;
	mdParticleCharge = mpParticleDefinition->GetPDGCharge();
}




////////////////////////////////////////////////////////////////
// DECAYS
void
NSParticleSource::CreatePrimaryParticle(G4PrimaryVertex * vtx)
{
	G4double mass = mpParticleDefinition->GetPDGMass();
	G4double energy = mdParticleEnergy + mass;
	G4double pmom = sqrt(energy * energy - mass * mass);
	G4double px = pmom * mhParticleMomentumDirection.x();
	G4double py = pmom * mhParticleMomentumDirection.y();
	G4double pz = pmom * mhParticleMomentumDirection.z();

	primEnergies[NofPrimaries] = mdParticleEnergy;
	NofPrimaries++;
	emit_energy += mdParticleEnergy;

	G4PrimaryParticle *particle = new G4PrimaryParticle(mpParticleDefinition, px, py, pz);
	particle->SetMass(mass);
	particle->SetCharge(mdParticleCharge);
	particle->SetPolarization(mhParticlePolarization.x(), mhParticlePolarization.y(), mhParticlePolarization.z());
	vtx->SetPrimary(particle);
}


void
NSParticleSource::GenerateKr83mDecay(G4PrimaryVertex * vert)
{
	G4double rndm1;
	G4double rndm2;

	rndm1 = G4UniformRand();
	rndm2 = G4UniformRand();
	
	// case 1
	if(rndm1<=0.95)
	{
		if(rndm2<=0.76)
		{	// 1st
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 7.6 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 1.8 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 30.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
		else if(rndm2>0.76 && rndm2<=0.85)
		{	// 1st
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 7.6 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 1.8 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 18.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 10.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
		else if(rndm2>0.85)
		{	// 1st
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 7.6 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 1.8 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 18.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 12.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
	}
	// case 2
	else if(rndm1>0.95)
	{
		if(rndm2<=0.76)
		{	// 1st
			particle = particleTable->FindParticle("gamma");
			SetParticleDefinition(particle);
			mdParticleEnergy = 9.4 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 30.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
		else if(rndm2>0.76 && rndm2<=0.85)
		{	// 1st
			particle = particleTable->FindParticle("gamma");
			SetParticleDefinition(particle);
			mdParticleEnergy = 9.4 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 18.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 10.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
		else if(rndm2>0.85)
		{	// 1st
			particle = particleTable->FindParticle("gamma");
			SetParticleDefinition(particle);
			mdParticleEnergy = 9.4 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			// 2nd
			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 18.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 12.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);

			particle = particleTable->FindParticle("e-");
			SetParticleDefinition(particle);
			mdParticleEnergy = 2.0 * CLHEP::keV;	
			CreatePrimaryParticle(vert);
			return;
		}
	}
}
// DECAYS
////////////////////////////////////////////////////////////




void
NSParticleSource::GeneratePrimaryVertex(G4Event * evt)
{

	if(mpParticleDefinition == 0)
	{
		G4cout << "No particle has been defined!" << G4endl;
		return;
	}

	// Position
	G4bool srcconf = false;
	G4int LoopCount = 0;
	
	if (NSStorage::Get()->GetGenerateInAm241()) GenerateInAm241();
	//ConfineSourceToVolume(mhVolumeNames);

	while(srcconf == false)
	{
		if(mhSourcePosType == "Point")
			GeneratePointSource();
		else if (mhSourcePosType == "Plane")
			GeneratePointsInPlane();
		else if(mhSourcePosType == "Surface")
			GeneratePointsInSurface();
		else if(mhSourcePosType == "Volume")
			GeneratePointsInVolume();
		else
		{
			G4cout << "Error: SourcePosType undefined" << G4endl;
			G4cout << "Generating point source" << G4endl;
			GeneratePointSource();
		}

		if(mbConfine == true)
		{
			srcconf = IsSourceConfined();
			// if source in confined srcconf = true terminating the loop
			// if source isnt confined srcconf = false and loop continues
		}
		else if(mbConfine == false)
			srcconf = true;		// terminate loop

		LoopCount++;
		if(LoopCount == 1000000)
		{
			G4cout << "*************************************" << G4endl;
			G4cout << "LoopCount = 1000000" << G4endl;
			G4cout << "Either the source distribution >> confinement" << G4endl;
			G4cout << "or any confining volume may not overlap with" << G4endl;
			G4cout << "the source distribution or any confining volumes" << G4endl;
			G4cout << "may not exist" << G4endl;
			G4cout << "If you have set confine then this will be ignored" << G4endl;
			G4cout << "for this event." << G4endl;
			G4cout << "*************************************" << G4endl;
			srcconf = true;		//Avoids an infinite loop
		}
	}

	// Angular stuff
	if(mhAngDistType == "iso")
		GenerateIsotropicFlux();
	else if(mhAngDistType == "direction")
		SetParticleMomentumDirection(mhParticleMomentumDirection);
	else
		G4cout << "Error: AngDistType has unusual value" << G4endl;

	// create a new vertex
	G4PrimaryVertex *vertex = new G4PrimaryVertex(mhParticlePosition, mdParticleTime);

	// Energy stuff
	if(mhEnergyDisType == "Mono")
		GenerateMonoEnergetic();
	else if(mhEnergyDisType == "Kr83m")
		GenerateKr83mDecay(vertex);
	else
		G4cout << "Error: EnergyDisType has unusual value" << G4endl;

	if(miVerbosityLevel >= 2)
		G4cout << "Creating primaries and assigning to vertex" << G4endl;
	// create new primaries and set them to the vertex
	G4double mass = mpParticleDefinition->GetPDGMass();
	G4double energy = mdParticleEnergy + mass;
	G4double pmom = std::sqrt(energy * energy - mass * mass);
	G4double px = pmom * mhParticleMomentumDirection.x();
	G4double py = pmom * mhParticleMomentumDirection.y();
	G4double pz = pmom * mhParticleMomentumDirection.z();

	if(miVerbosityLevel >= 1)
	{
		G4cout << "Particle name: " << mpParticleDefinition->GetParticleName() << G4endl;
		G4cout << "       Energy: " << mdParticleEnergy << G4endl;
		G4cout << "     Position: " << mhParticlePosition << G4endl;
		G4cout << "    Direction: " << mhParticleMomentumDirection << G4endl;
		G4cout << " NumberOfParticlesToBeGenerated: " << miNumberOfParticlesToBeGenerated << G4endl;
	}

	for(G4int i = 0; i < miNumberOfParticlesToBeGenerated; i++)
	{
		G4PrimaryParticle *particle = new G4PrimaryParticle(mpParticleDefinition, px, py, pz);
		particle->SetMass(mass);
		particle->SetCharge(mdParticleCharge);
		particle->SetPolarization(mhParticlePolarization.x(), mhParticlePolarization.y(), mhParticlePolarization.z());
		vertex->SetPrimary(particle);
	}
	evt->AddPrimaryVertex(vertex);
	if(miVerbosityLevel > 1)
		G4cout << " Primary Vertex generated " << G4endl;
}

void
NSParticleSource::GeneratePrimaryVertexFromTrack(G4Track *pTrack, G4Event *pEvent)
{
	G4double dPX = pTrack->GetMomentum().x();
	G4double dPY = pTrack->GetMomentum().y();
	G4double dPZ = pTrack->GetMomentum().z();

	G4PrimaryVertex *pVertex = new G4PrimaryVertex(pTrack->GetPosition(), mdParticleTime);

	G4PrimaryParticle *pPrimary = new G4PrimaryParticle(pTrack->GetDefinition(), dPX, dPY, dPZ);
	pPrimary->SetMass(pTrack->GetDefinition()->GetPDGMass());
	pPrimary->SetCharge(pTrack->GetDefinition()->GetPDGCharge());

	pVertex->SetPrimary(pPrimary);

	pEvent->AddPrimaryVertex(pVertex);
}


	
