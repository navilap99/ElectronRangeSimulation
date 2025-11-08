/// \file B1/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the B1::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace B1{

    PrimaryGeneratorAction::PrimaryGeneratorAction(){

        G4int n_particle = 1;
        fParticleGun = new G4ParticleGun(n_particle);

        // Default kinematics
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4String particleName;
        G4ParticleDefinition* particle = particleTable->FindParticle(particleName = "e-");
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
        fParticleGun->SetParticleEnergy(10. * MeV);

    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction(){

        delete fParticleGun;

    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event){

        G4double randomTheta = CLHEP::pi * G4UniformRand();
        G4double randomPhi = 2 * CLHEP::pi * G4UniformRand();
        G4double directionX = std::sin(randomTheta) * std::cos(randomPhi);
        G4double directionY = std::sin(randomTheta) * std::sin(randomPhi);
        G4double directionZ = std::cos(randomTheta);

        fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(directionX, directionY, directionZ));

        fParticleGun->GeneratePrimaryVertex(event);

    }

}