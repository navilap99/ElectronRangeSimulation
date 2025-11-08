/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"

namespace B1{

    G4VPhysicalVolume* DetectorConstruction::Construct(){

        // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

        // Option to switch on/off checking of volumes overlaps
        G4bool checkOverlaps = true;

        // **********************
        // Sphere parameters 
        // **********************

        // Define material

        G4Material* sphereMat = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRU-4");

        // **********************
        // World parameters 
        // **********************

        G4double worldRadius = 1.2 * sphereRadius;
        G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

        // **********************
        // Solids, Logicals and Placement
        // **********************

        // World

        auto solidWorld = new G4Sphere(
            "World",
            0.,
            worldRadius,
            0,
            360 * deg,
            0,
            180 * deg
        );

        auto logicWorld = new G4LogicalVolume(
            solidWorld,
            worldMat,
            "World" // its name
        );

        auto physWorld = new G4PVPlacement(
            nullptr,  // no rotation
            G4ThreeVector(),
            logicWorld,  // its logical volume
            "World",  // its name
            nullptr,  // its mother  volume
            false,  // no boolean operation
            0,  // copy number
            checkOverlaps
        );

        // Sphere

        auto solidSphere = new G4Sphere(
            "Sphere",
            0.0,
            sphereRadius,
            0,
            360 * deg,
            0,
            180 * deg
        );

        auto logicSphere = new G4LogicalVolume(
            solidSphere,
            sphereMat,
            "Sphere"
        );

        new G4PVPlacement(
            nullptr,
            G4ThreeVector(),
            logicSphere,
            "Sphere",
            logicWorld,
            false,
            0,
            checkOverlaps
        );
        
        fScoringVolume = logicSphere;

        return physWorld;
        
    }

}
