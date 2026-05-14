// ============================================================
// shape.h — Defines the abstract Shape base class.
// Every drawable object (Line, Circle, Rectangle, etc.) is a
// subclass of Shape. AppManager only ever holds Shape* pointers,
// so it can treat every shape identically regardless of type.
// This is the classic OOP "polymorphism" pattern.
// ============================================================

#pragma once          // Prevent this header from being included more than once per translation unit
#ifndef SHAPE_H       // Classic guard for older compilers — same effect as #pragma once
#define SHAPE_H       // Define the guard symbol

#include "point.h"    // Point class — shapes store their defining points in a vector<Point>
#include <string>     // std::string — used for human-readable name and description of each shape
#include <vector>     // std::vector — used to store the list of points that define the shape
#include <windows.h>  // COLORREF — the Windows type for storing an RGB color as a single 32-bit integer
#include <cmath>      // Mathematical functions (sqrt, fabs, etc.) used in geometry calculations
#include <iostream>   // std::cout / std::cerr — for debug output when printing shape state
#include "algorithm"  // std::sort / std::stable_sort — used when ordering points (e.g. for polygon centroid)

// ── Enum: identifies which concrete shape subclass an object is ─
// This avoids expensive dynamic_cast<> at runtime when we need to branch on shape type.
enum ShapeType {
    SHAPE_NONE,             // Placeholder / uninitialized
    SHAPE_POLYGON,          // Generic polygon (N vertices)
    SHAPE_LINE,             // Two-point straight line
    SHAPE_CIRCLE,           // Center point + one edge point to define radius
    SHAPE_ELLIPSE,          // Center + two radii points
    SHAPE_CURVE,            // Bézier / spline curve defined by control points
    SHAPE_RECTANGLE,        // Opposite-corner rectangle
    SHAPE_SQUARE,           // Equal-sided rectangle
    SHAPE_HAPPY_SMILE_FACE, // Composite decorative shape
    SHAPE_SAD_SMILE_FACE    // Composite decorative shape
};

// ── Abstract base class ──────────────────────────────────────
class Shape {
private:
    // (nothing private — subclasses and Shape itself share everything below)
protected:
    double area; // Pre-computed area of the shape; used when sorting overlapping shapes so the smallest is filled first
    Shape(ShapeType type, std::string name, std::string description); // Constructor is protected — you cannot create a bare Shape; you must use a subclass
    std::string name;        // Short label shown in the status printout (e.g. "Circle", "Line")
    std::string description; // Longer human-readable string shown when the shape is selected (e.g. "Click center, then edge")
    ShapeType type;          // Which enum value this shape is — lets code branch without dynamic_cast
    Point centroid(const std::vector< Point > &givenPoints) const; // Helper: computes the geometric center of a set of points
                                                                    // Used by fill algorithms that need a seed point inside the shape
public:
    std::vector<Point> points; // The defining points the user has clicked so far (e.g. [center, edge] for a circle)
    COLORREF borderColor;      // The RGB color used to draw the shape's outline (set from the color picker)
    COLORREF fillColor;        // The RGB color used to fill the shape's interior (set from the color picker)

    bool addPoint(const Point &point); // Adds the next user-clicked point; returns true when the shape now has enough to draw
    void clear();                      // Removes all stored points so the shape can be reused for a new drawing
    std::string getDescription() const; // Returns the instruction string shown to the user (e.g. "Click two corners")
    std::string getName() const;        // Returns the short name (e.g. "Rectangle")
    ShapeType getType() const;          // Returns the ShapeType enum value for fast type checks
    int getSize() const;                // Returns how many points have been collected so far

    virtual void takeAction(int actionID); // Optional hook called with a right-click ID; used by curves to "close" themselves
                                           // Default implementation does nothing; CurveShape overrides it

    // ── Pure virtual methods — every subclass MUST implement these ──

    virtual void initialize() = 0;  // Called after all points are collected; subclass pre-computes derived values (e.g. radius)
                                    // The = 0 makes Shape abstract — you cannot instantiate Shape directly

    virtual bool isInside(const Point &point) const = 0; // Returns true if the given screen point lies inside this shape
                                                          // Used by right-click fill: find which shape the user clicked inside

    virtual bool isEnoughToDraw() const = 0; // Returns true when enough points have been collected to draw the shape
                                             // e.g. a Line needs exactly 2 points; a Circle needs exactly 2 points

    virtual Shape* clone() const = 0; // Creates and returns a deep copy of this shape
                                      // Needed by AppManager when it clones a shape to start drawing a new instance of the same type

    virtual std::vector< Point > getSidePoints() const = 0; // Returns a list of points along the shape's boundary
                                                             // Used by clipping algorithms to test intersection with a clipping region

    double getArea() const; // Returns the pre-computed area value
                            // Used to sort shapes so right-click fill targets the smallest enclosing shape
};

#endif // SHAPE_H  // End of the include guard
