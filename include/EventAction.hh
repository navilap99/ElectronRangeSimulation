/// \file B1/include/EventAction.hh
/// \brief Definition of the B1::EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

namespace B1{

    class RunAction;

    class EventAction : public G4UserEventAction{

    public:

        EventAction(RunAction* runAction);
        ~EventAction() override = default;

        void BeginOfEventAction(const G4Event* event) override;
        void EndOfEventAction(const G4Event* event) override;

        void addDistance(G4double distance) { traveledDistance += distance; }

    private:

        RunAction* fRunAction = nullptr;
        G4double traveledDistance = 0.;

    };

}

#endif
