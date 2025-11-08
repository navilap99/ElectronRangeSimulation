/// \file B1/include/DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace B1{

    class DetectorConstruction : public G4VUserDetectorConstruction{

        public:

            DetectorConstruction() = default;
            ~DetectorConstruction() override = default;

            G4VPhysicalVolume* Construct() override;

            G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

        protected:

            G4double sphereRadius = 2. * m;
            G4LogicalVolume* fScoringVolume = nullptr;

    };

}

#endif
