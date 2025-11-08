/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <filesystem>

namespace B1{

    RunAction::RunAction(){

        G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
        accumulableManager->Register(totalDistance);
        accumulableManager->Register(totalDistance2);

    }

    void RunAction::BeginOfRunAction(const G4Run*){

        G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
        accumulableManager->Reset();

    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void RunAction::EndOfRunAction(const G4Run* run){

        G4int nofEvents = run->GetNumberOfEvent();
        if (nofEvents == 0) return;

        // Merge accumulables
        G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
        accumulableManager->Merge();

        if (IsMaster()) {

            double meanDistance = totalDistance.GetValue() / (nofEvents + 0.0);
            double meanDistance2 = totalDistance2.GetValue() / (nofEvents + 0.0);

            G4double rmsDistance = meanDistance2 - pow(meanDistance, 2.0);
            if (rmsDistance > 0.) rmsDistance = std::sqrt(rmsDistance); else rmsDistance = 0.;

            // Write output file
            std::string filename = "output.txt";
            if (std::filesystem::exists(filename)) {

                std::ofstream file;
                file.open(filename, std::ios::app);
                file << meanDistance / cm << "\t" << rmsDistance / cm << "\n";
                file.close();

            }

            // Print
            G4cout
            << "Mean traveled distance: "
            << meanDistance / cm << " cm with rms = " << rmsDistance / cm
            << G4endl
            << "------------------------------------------------------------"
            << G4endl
            << G4endl;

        }

    }

    void RunAction::addDistance(G4double distance){

        totalDistance += distance;
        totalDistance2 += distance * distance;

    }

}