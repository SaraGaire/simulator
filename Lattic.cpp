#include "LBC.h"
#include <cmath>

//velocity directions: center, cardinal, diagonal
const int Lattice::cx[D2Q9_DIRECTIONS] = {0, 1, 0, -1, 0, 1, -1, -1, 1};
const int Lattice::cy[D2Q9_DIRECTIONS] = {0, 0, 1, 0, -1, 1, 1, -1, -1};

// weights pf the equilibrium distribution
const double Lattice::weights[D2Q9_DIRECTIONS] = {
    4.0/9.0, 
    1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0,  
    1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0  
};
//directionf or the cemnter, diagonal and the cardinal.

Lattice::Lattice(int w, int h, double visc, double inlet_vel) 
    : width(w), height(h), viscosity(visc), inlet_velocity(inlet_vel) {
    
    //  relaxation time from viscosity
    // tau = 3 * viscosity + 0.5 (LBM relationship)
    tau = 3.0 * viscosity + 0.5;
    
    // grid initializationand equillibrium initialization.
    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width);
    }
    
  
    initializeEquilibrium();
}

void Lattice::setObstacle(int x, int y, bool is_obstacle) {
    if (isValidPosition(x, y)) {
        grid[y][x].is_obstacle = is_obstacle;
    }
}

void Lattice::setInlet(int x, int y, bool is_inlet) {
    if (isValidPosition(x, y)) {
        grid[y][x].is_inlet = is_inlet;
    }
}

void Lattice::setOutlet(int x, int y, bool is_outlet) {
    if (isValidPosition(x, y)) {
        grid[y][x].is_outlet = is_outlet;
    }
}

void Lattice::setupChannelFlow() {
    //clearing all the exisiting boundaries.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x].is_obstacle = false;
            grid[y][x].is_inlet = false;
            grid[y][x].is_outlet = false;
        }
    }
    
    //obstacles setting
    for (int x = 0; x < width; x++) {
        setObstacle(x, 0, true);           // Bottom wall
        setObstacle(x, height - 1, true);  // Top wall
    }
    
  
    for (int y = 1; y < height - 1; y++) {
        setInlet(0, y, true);
    }
    
    
    for (int y = 1; y < height - 1; y++) {
        setOutlet(width - 1, y, true);
    }
}

void Lattice::setupCylinderFlow() {
    setupChannelFlow(); // channel flow
    
    //  circular obstacle in the middle
    int center_x = width / 3;
    int center_y = height / 2;
    int radius = height / 8;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dx = x - center_x;
            double dy = y - center_y;
            double distance = sqrt(dx * dx + dy * dy);
            
            if (distance <= radius) {
                setObstacle(x, y, true);
                //  inlet/outlet flags  removing if they were set
                grid[y][x].is_inlet = false;
                grid[y][x].is_outlet = false;
            }
        }
    }
}

void Lattice::setupCavityFlow() {
    // bounaries clearizatoipn
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x].is_obstacle = false;
            grid[y][x].is_inlet = false;
            grid[y][x].is_outlet = false;
        }
    }
    
    //  obstacles setting
    for (int x = 0; x < width; x++) {
        setObstacle(x, 0, true);          
        setObstacle(x, height - 1, true); 
    }
    
    for (int y = 0; y < height; y++) {
        setObstacle(0, y, true);           
        setObstacle(width - 1, y, true);   
    }
}

void Lattice::initializeEquilibrium() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LatticePoint& point = grid[y][x];
            
            //  initial conditions based on boundary type
            if (point.is_inlet) {
                point.density = 1.0;
                point.velocity = Vector2D(inlet_velocity, 0.0);
            } else {
                point.density = 1.0;
                point.velocity = Vector2D(0.0, 0.0);
            }
            
            //equilibrium distribution calculation is doen here
            for (int i = 0; i < D2Q9_DIRECTIONS; i++) {
                point.f[i] = calculateEquilibrium(i, point.density, point.velocity);
                point.f_new[i] = point.f[i];
            }
        }
    }
}

double Lattice::calculateEquilibrium(int i, double density, const Vector2D& velocity) const {
    // LBM equilibrium distribution function using the foirmula 
    double u_dot_ci = velocity.x * cx[i] + velocity.y * cy[i];
    double u_squared = velocity.x * velocity.x + velocity.y * velocity.y;
    
    double equilibrium = weights[i] * density * (
        1.0 + 3.0 * u_dot_ci + 
        4.5 * u_dot_ci * u_dot_ci - 
        1.5 * u_squared
    );
    
    return equilibrium;
}

bool Lattice::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}