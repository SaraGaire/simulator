#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

// here we will be creating a simple 2d vector which will provide a basic operation needed for the  LBM.

class Vector2D {
public:
    double x, y;
    
    //2constructors
    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double x, double y) : x(x), y(y) {}
    
    // operations
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    // dot product
    double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    

    double magnitude() const {
        return sqrt(x * x + y * y);
    }
    
    //normalization of a vector
    Vector2D normalized() const {
        double mag = magnitude();
        if (mag > 0.0) {
            return Vector2D(x / mag, y / mag);
        }
        return Vector2D(0.0, 0.0);
    }
};

#endif // VECTOR2D_H