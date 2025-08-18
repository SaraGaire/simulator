#ifndef LATTICE_H
#define LATTICE_H

#include "Vector.h"
#include <vector>

/**
 * Lattice point for D2Q9 Lattice Boltzmann Method
 * D2Q9 = 2D space with 9 velocity directions
 */
struct LatticePoint {
    // Distribution functions (9 directions for D2Q9)
    double f[9];      // Current distribution
    double f_new[9];  // New distribution after collision
    
    // Macroscopic properties
    double density;   // Fluid density
    Vector2D velocity; // Fluid velocity
    
    // Boundary conditions
    bool is_obstacle; // True if this is a solid boundary
    bool is_inlet;    // True if this is a velocity inlet
    bool is_outlet;   // True if this is a pressure outlet
    
    // Constructor
    LatticePoint() : density(1.0), velocity(0.0, 0.0), 
                     is_obstacle(false), is_inlet(false), is_outlet(false) {
        // Initialize all distributions to equilibrium
        for (int i = 0; i < 9; i++) {
            f[i] = f_new[i] = 0.0;
        }
    }
};

/**
 * Main lattice grid for LBM simulation
 */
class Lattice {
public:
    static const int D2Q9_DIRECTIONS = 9;
    
    // D2Q9 velocity vectors (including zero velocity at center)
    static const int cx[D2Q9_DIRECTIONS];
    static const int cy[D2Q9_DIRECTIONS];
    
    // D2Q9 weights for equilibrium distribution
    static const double weights[D2Q9_DIRECTIONS];
    
private:
    int width, height;
    std::vector<std::vector<LatticePoint>> grid;
    
    // Physical parameters
    double tau;           // Relaxation time
    double viscosity;     // Kinematic viscosity
    double inlet_velocity; // Inlet velocity magnitude
    
public:
    Lattice(int w, int h, double viscosity = 0.1, double inlet_vel = 0.1);
    
    // Grid access
    LatticePoint& getPoint(int x, int y) { return grid[y][x]; }
    const LatticePoint& getPoint(int x, int y) const { return grid[y][x]; }
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    double getTau() const { return tau; }
    double getInletVelocity() const { return inlet_velocity; }
    
    // Boundary setup
    void setObstacle(int x, int y, bool is_obstacle = true);
    void setInlet(int x, int y, bool is_inlet = true);
    void setOutlet(int x, int y, bool is_outlet = true);
    
    // Setup common geometries
    void setupChannelFlow();
    void setupCylinderFlow();
    void setupCavityFlow();
    
    // Simulation methods
    void initializeEquilibrium();
    double calculateEquilibrium(int i, double density, const Vector2D& velocity) const;
    bool isValidPosition(int x, int y) const;
};

#endif // LATTICE_H