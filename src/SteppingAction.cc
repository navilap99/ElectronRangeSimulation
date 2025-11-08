/// \file B1/src/SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"

namespace B1{

    SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

    void SteppingAction::UserSteppingAction(const G4Step* step){

        if (!fScoringVolume) {

            const auto detConstruction = static_cast<const DetectorConstruction*>(
                G4RunManager::GetRunManager()->GetUserDetectorConstruction()
            );
            fScoringVolume = detConstruction->GetScoringVolume();

        }

        // Get volume of the current step
        G4LogicalVolume* volume =
            step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

        // Return if not in volume
        if (volume != fScoringVolume) return;

        G4Track* track = step->GetTrack();
        if (track->GetParentID() == 0) {
            
            G4double stepLength = step->GetStepLength();
            fEventAction->addDistance(stepLength);

        }

        if (track->GetKineticEnergy() == 0 || track->GetTrackStatus() == fStopAndKill) {

            return;

        }

    }

}
