// ============================================================
// point.h — Defines the Point class.
// A Point represents a 2D coordinate (x, y) on the canvas.
// It is the fundamental building block used by every shape,
// algorithm, and clipping operation in the project.
// ============================================================

#pragma once         // Modern way to prevent this file from being included more than once per compilation unit
#ifndef POINT_H      // Classic include guard — same purpose as #pragma once; both together are extra-safe
#define POINT_H      // Define the guard symbol so the second check above will be false next time
#include <cmath>     // Needed for sqrt() (Euclidean distance) and atan2() (angle calculation)

class Point {
public:
    double x, y; // The horizontal (x) and vertical (y) screen coordinates; double for sub-pixel precision in algorithms

    // ── Constructors ───────────────────────────────────────────
    Point() : x(0), y(0) {}                          // Default constructor — creates the origin point (0,0)
    Point(double _x, double _y) : x(_x), y(_y) {}   // Main constructor — creates a point at (_x, _y)

    // ── Destructor ─────────────────────────────────────────────
    ~Point() {} // Nothing heap-allocated inside Point, so destructor body is empty

    // ── Difference helpers (used heavily by drawing algorithms) ─
    double dx(const Point& other) const {
        return other.x - x; // Horizontal distance FROM this point TO other (positive = other is to the right)
    }

    double dy(const Point& other) const {
        return other.y - y; // Vertical distance FROM this point TO other (positive = other is below, since y grows downward on screen)
    }

    double slope(const Point& other) const {
        if(dx(other) == 0) {                                  // Guard against division by zero for vertical lines
            return std::numeric_limits<double>::infinity();   // Return infinity — mathematically correct slope for a vertical line
        }
        return dy(other) / dx(other); // rise / run = classic slope formula
    }

    // ── Distance metrics ───────────────────────────────────────
    long long manhattanDistance(const Point& other) const {
        // Manhattan distance = |dx| + |dy|; fast because it needs no sqrt
        // Used where an approximate "how far" is enough (e.g., quick checks)
        return (long long)dx(other) + (long long)dy(other);
    }

    double euclideanDistance(const Point& other) const {
        // True straight-line distance using the Pythagorean theorem: sqrt(dx² + dy²)
        // Used when accuracy matters (e.g., computing circle radius from two clicked points)
        return sqrt(dx(other) * dx(other) + dy(other) * dy(other));
    }

    // ── Linear Interpolation ───────────────────────────────────
    Point lerp(const Point& b, double t) const {
        // Returns the point that is t% of the way from *this to b.
        // t=0.0 returns *this exactly; t=1.0 returns b exactly; t=0.5 is the midpoint.
        // Used in curve/spline drawing to generate smooth intermediate points.
        return *this + (b - *this) * t;
    }

    // ── Arithmetic operators ───────────────────────────────────
    // These let us write "p1 + p2" instead of "Point(p1.x+p2.x, p1.y+p2.y)"

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y); // Adds two points component-wise (used in lerp and translations)
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y); // Subtracts two points (used to get direction vector between them)
    }

    Point operator*(const Point& other) const {
        return Point(x * other.x, y * other.y); // Component-wise multiplication (scales each axis independently)
    }

    Point operator*(double factor) const {
        return Point(x * factor, y * factor); // Scales both x and y by the same scalar factor (used in lerp)
    }

    // ── Compound assignment operators ──────────────────────────
    // These modify the point in-place (more efficient when you don't need a new object)

    Point& operator+=(const Point& other) {
        x += other.x; // Add other's x to this point's x
        y += other.y; // Add other's y to this point's y
        return *this; // Return reference to self so calls can be chained: p1 += p2 += p3
    }

    Point& operator-=(const Point& other) {
        x -= other.x; // Subtract other's x
        y -= other.y; // Subtract other's y
        return *this; // Return self for chaining
    }

    Point& operator*=(const Point& other) {
        x *= other.x; // Multiply x component in-place
        y *= other.y; // Multiply y component in-place
        return *this; // Return self for chaining
    }

    Point& operator*=(double factor) {
        x *= factor; // Scale x in-place
        y *= factor; // Scale y in-place
        return *this; // Return self for chaining
    }

    // ── Comparison operator ────────────────────────────────────
    bool operator<(const Point& other) const {
        double angle1 = atan2(y, x);       // Compute polar angle of THIS point from the origin
        double angle2 = atan2(other.y, other.x); // Compute polar angle of OTHER point from the origin
        return angle1 > angle2; // NOTE: intentionally reversed (>) so sorting goes clockwise instead of counter-clockwise
        // Used when sorting polygon vertices so they are ordered consistently for fill algorithms
    }
};

#endif //POINT_H  // End of the include guard
