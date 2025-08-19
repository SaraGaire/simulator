#ifndef LBM_SIMULATOR_H
#define LBM_SIMULATOR_H

#include "Lattice.h"
#include "Vector2D.h"

/**
 * Lattice Boltzmann Method Fluid Simulator
 * Implements the collision-streaming algorithm for fluid dynamics
 */
class LBMSimulator {
private:
    Lattice* lattice;
    int time_step;
    
    // Simulation parameters
    double lid_velocity;  // For cavity flow
    bool is_running;
    
    // Performance tracking
    double average_density;
    Vector2D average_velocity;
    
public:
    LBMSimulator(Lattice* lat);
    ~LBMSimulator();
    
    // Main simulation steps
    void step();
    void collision();
    void streaming();
    void applyBoundaryConditions();
    
    // Boundary condition implementations
    void bounceBackBoundary();
    void velocityInletBoundary();
    void pressureOutletBoundary();
    void movingWallBoundary();
    
    // Macroscopic property calculation
    void calculateMacroscopicProperties();
    void updateMacroscopicProperties(int x, int y);
    
    // Simulation control
    void start() { is_running = true; }
    void stop() { is_running = false; }
    void reset();
    
    // Getters
    bool isRunning() const { return is_running; }
    int getTimeStep() const { return time_step; }
    double getAverageDensity() const { return average_density; }
    Vector2D getAverageVelocity() const { return average_velocity; }
    Lattice* getLattice() { return lattice; }
    
    // Setters
    void setLidVelocity(double vel) { lid_velocity = vel; }
    
    // Analysis methods
    double calculateKineticEnergy() const;
    double calculateTotalMass() const;
    Vector2D calculateMomentum() const;
    
private:
    // Helper methods
    Vector2D getOppositeDirection(int direction) const;
    int getOppositeDirectionIndex(int i) const;
};

#endif // LBM_SIMULATOR_H