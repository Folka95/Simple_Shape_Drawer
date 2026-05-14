// ============================================================
// happy_smile_face_drawing_algorithm.cpp — Happy Smile Face
//
// CORE IDEA: Draws a composite face made of 5 components,
// all calculated relative to a center point and radius that
// the user defines with 2 clicks:
//   1. FACE OUTLINE  — a large full circle
//   2. LEFT EYE      — small circle at (-r/3, -r/4) from center
//   3. RIGHT EYE     — small circle at (+r/3, -r/4) from center
//   4. NOSE          — two lines forming an upside-down V shape
//   5. MOUTH         — an arc from 200° to 340° using the polar formula
//                      (smiling = arc curves upward, using abs(sin))
//
// All positions and sizes are proportional to the face radius,
// so the face scales correctly regardless of how big the user draws it.
// ============================================================

#include "happy_smile_face_drawing_algorithm.h"           // Header declaring this class
#include <cmath>                                           // M_PI, cos, sin, abs, round
#include "../circle/circle_midpoint_drawing_algorithm.h"  // Used to draw the face outline and eyes
#include "../../../core/shapes/circle.h"                  // Circle shape — used as a container for each circular component
#include "../../../core/shapes/curve.h"                   // CurveShape — included for completeness (not directly used here)
#include "../../../core/shapes/line.h"                    // Line shape — used for the nose lines
#include "../curve/curve_spline_drawing_algorithm.h"       // Spline — included but not used in this version
#include "../line/line_midpoint_drawing_algorithm.h"       // Midpoint line algo — used for the nose

// Constructor
HappySmileFace_DrawingAlgorithm::HappySmileFace_DrawingAlgorithm()
    : DrawingAlgorithm("HappySmileFace_DrawingAlgorithm") {
}

// ── Helper: draw one circle component of the face ────────────
void HappySmileFace_DrawingAlgorithm::drawCircle(HappySmileFace* face, const Point& center, double radius, ScreenWriter *sw) const {
    Shape *circle = new Circle(); // Create a temporary Circle shape to hold this component
    circle->addPoint(center);                          // Set the center of this circle
    circle->addPoint(Point(center.x + radius, center.y)); // Edge point = center + (radius, 0); defines the radius
    DrawingAlgorithm *algo = new Circle_MidPoint_DrawingAlgorithm(); // Use Bresenham midpoint for efficiency
    circle->borderColor = face->borderColor; // Inherit the face's border color
    circle->fillColor   = face->fillColor;   // Inherit the face's fill color
    algo->draw(*circle, sw); // Draw the circle to the screen
}

// ── Helper: draw the nose ─────────────────────────────────────
// The nose is two line segments forming a small V-shape at the face center
void HappySmileFace_DrawingAlgorithm::drawNose(HappySmileFace* face, double radius, ScreenWriter *sw) const {
    Point c = face->points[0]; // Face center

    // Three nose points calculated as fractions of the radius
    Point p1(c.x,                c.y + radius / 8);   // Bottom tip of the nose (center, slightly below center)
    Point p2(c.x - radius / 12,  c.y - radius / 16);  // Upper-left point of the V
    Point p3(c.x + radius / 12,  c.y - radius / 16);  // Upper-right point of the V

    Line l1; // Left side of the V: p1 → p2
    l1.borderColor = face->borderColor;
    l1.addPoint(p1); // Bottom tip
    l1.addPoint(p2); // Upper-left

    Line l2; // Right side of the V: p2 → p3
    l2.borderColor = face->borderColor;
    l2.addPoint(p2); // Upper-left
    l2.addPoint(p3); // Upper-right

    DrawingAlgorithm *algo = new Line_Midpoint_DrawingAlgorithm();
    algo->draw(l1, sw); // Draw left line of nose
    algo->draw(l2, sw); // Draw right line of nose
}

// ── Helper: draw the mouth (happy arc) ───────────────────────
// A smiling mouth is an arc that curves UPWARD.
// Uses the polar method: sweep angle from 200° to 340°.
// The abs(sin) makes the y-component always positive (below center),
// creating the characteristic upward curve of a smile.
void HappySmileFace_DrawingAlgorithm::drawMouth(HappySmileFace* face, double radius, ScreenWriter *sw) const {
    sw->activate(); // Acquire DC before writing pixels directly

    for(double angle = 200.0; angle <= 340.0; angle += 0.1) { // Sweep 140° arc from bottom-left to bottom-right
        double rad = angle * M_PI / 180.0; // Convert degrees to radians

        double x = face->points[0].x + radius / 1.8 * cos(rad);   // x: slightly narrower than full radius (÷1.8 shrinks the width)
        double y = face->points[0].y + radius / 2  * abs(sin(rad)); // y: abs(sin) keeps y below center, creating upward curve
                                                                     // radius/2 scales the mouth height

        sw->setPixel(round(x), round(y), face->borderColor); // Plot the mouth pixel
    }

    sw->deactivate(); // Release DC
}

// ── Main assembly: draw all face components ───────────────────
void HappySmileFace_DrawingAlgorithm::runAlgorithm(HappySmileFace* face, ScreenWriter *sw) const {
    Point center = face->points[0]; // Center point (first click)
    Point edge   = face->points[1]; // Edge point (second click — used to compute radius)
    double radius = face->points[0].euclideanDistance(face->points[1]); // Radius = distance between the two clicks

    drawCircle(face, face->points[0], radius, sw); // 1. Draw the main face outline

    // 2. Draw left eye: positioned at (-r/3, -r/4) relative to center (upper-left area)
    drawCircle(face, Point(center.x - radius / 3, center.y - radius / 4), radius / 8, sw);

    // 3. Draw right eye: positioned at (+r/3, -r/4) relative to center (upper-right area)
    drawCircle(face, Point(center.x + radius / 3, center.y - radius / 4), radius / 8, sw);

    drawNose(face, radius, sw);  // 4. Draw the nose
    drawMouth(face, radius, sw); // 5. Draw the smiling mouth arc
}

// ── Public entry point ────────────────────────────────────────
void HappySmileFace_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_HAPPY_SMILE_FACE) {
        std::cerr << "HappySmileFace_DrawingAlgorithm::draw : shape is expected to be happy smile face" << std::endl;
        return;
    }
    if(inputShape.points.size() < 2) {
        std::cerr << "HappySmileFace_DrawingAlgorithm::draw : not enough points" << std::endl;
        return; // Need center + edge point before drawing
    }
    HappySmileFace* face = dynamic_cast<HappySmileFace*>(inputShape.clone()); // Clone + cast to typed pointer
    this->runAlgorithm(face, sw);
}
