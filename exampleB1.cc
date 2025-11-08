/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "G4VModularPhysicsList.hh"
#include "G4LossTableManager.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4UnitsTable.hh"

#include <filesystem>
#include <vector>

using namespace B1;

int main(int argc, char** argv){

    G4RunManager* runManager = nullptr;

    std::vector<double> energies;
    int nEvents = 0;

    // Detect interactive mode and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {

        ui = new G4UIExecutive(argc, argv);
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);

    }

    else{

        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

        // Run parameters

        G4double energyMin = 0.01 * MeV;
        G4double energyMax = 10 * MeV;
        double energyStep = 0.1;
        nEvents = std::stoi(argv[1]);

        double indexMin = std::log10(energyMin / MeV);
        double indexMax = std::log10(energyMax / MeV);

        // Custom energies
        for (double index = indexMin; index <= (indexMax + energyStep); index += energyStep) {

            double energy = std::pow(10, index);
            double energyToPush = energy * ( 1 - 1/( 1 + 2*energy/0.511 ) );

            energies.push_back(energyToPush);

        }

    }

    // DetectorConstruction
    runManager->SetUserInitialization(new DetectorConstruction());

    // Physics list
    G4VModularPhysicsList* physicsList = new G4VModularPhysicsList();
	G4LossTableManager::Instance();
	physicsList->SetDefaultCutValue(0.01*mm);
	physicsList->RegisterPhysics(new G4EmStandardPhysics_option3);
	G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
	stepLimitPhys->SetApplyToAll(true);
	physicsList->RegisterPhysics(stepLimitPhys);
	runManager->SetUserInitialization(physicsList);

    // ActionInitialization
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize();

    // Initialize visualization with the default graphics system
    auto visManager = new G4VisExecutive(argc, argv);
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // **********
    // Output file
    // **********

    std::string filename = "output.txt";
    if (!std::filesystem::exists(filename)) {

        std::ofstream newFile(filename);

        if (newFile.is_open()) {

            newFile << "electronEnergy / MeV" << "\t" << "range / cm" << "\t" << "dRange / cm" << "\n";
            newFile.close();
            
        } 
    }

    // Process macro or start UI session
    if (!ui) {

        // Batch mode
        for (double e : energies) {

            G4double energy = e;

            if (std::filesystem::exists(filename)) {

                std::ofstream file;
                file.open(filename, std::ios::app);
                file << energy / MeV << "\t";
                file.close();

            }

            std::cout << energy << "\n";
            UImanager->ApplyCommand("/gun/particle e-");
            std::ostringstream energyCmd;
            energyCmd << "/gun/energy " << G4BestUnit(energy, "Energy");
            std::cout << energyCmd.str() << "\n";
            UImanager->ApplyCommand(energyCmd.str());

            std::ostringstream beamOnCmd;
            beamOnCmd << "/run/beamOn " << nEvents;

            G4cout
            << G4endl
            << "------------------------------------------------------------"
            << G4endl
            << "The run consists of " << nEvents << " electrons of energy " << G4BestUnit(energy, "Energy")
            << G4endl;

            UImanager->ApplyCommand(beamOnCmd.str());
            
        }

    }
    else {

        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;

    }

    delete visManager;
    delete runManager;
    
}