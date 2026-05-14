// ============================================================
// sad_smile_face_drawing_algorithm.cpp — Sad Smile Face
//
// CORE IDEA: Same structure as the happy face but the mouth
// arc is flipped so it curves DOWNWARD (a frown).
//
// Differences from the happy face:
//   NOSE:  The V-shape is mirrored — left and right upper
//          points are swapped, giving a reversed look.
//   MOUTH: The y formula uses a negative sin:
//            y = center.y - r/2 * abs(sin) + r/1.4
//          Subtracting (instead of adding) flips the arc upward
//          in screen coordinates. The +r/1.4 offset shifts the
//          whole mouth down so it sits below the center.
// ============================================================

#include "sad_smile_face_drawing_algorithm.h"              // Header declaring this class
#include <cmath>                                            // M_PI, cos, sin, abs, round
#include "../circle/circle_midpoint_drawing_algorithm.h"   // For drawing face outline and eyes
#include "../../../core/shapes/circle.h"                   // Circle shape container
#include "../../../core/shapes/line.h"                     // Line shape — for nose lines
#include "../line/line_midpoint_drawing_algorithm.h"        // Midpoint line algo for nose

// Constructor
SadSmileFace_DrawingAlgorithm::SadSmileFace_DrawingAlgorithm()
    : DrawingAlgorithm("SadSmileFace_DrawingAlgorithm") {
}

// ── Helper: draw one circle component ────────────────────────
void SadSmileFace_DrawingAlgorithm::drawCircle(SadSmileFace* face, const Point& center, double radius, ScreenWriter *sw) const {
    Shape *circle = new Circle();
    circle->addPoint(center);                              // Center of this circle component
    circle->addPoint(Point(center.x + radius, center.y)); // Edge point defines the radius
    DrawingAlgorithm *algo = new Circle_MidPoint_DrawingAlgorithm();
    circle->borderColor = face->borderColor; // Use the face's border color
    circle->fillColor   = face->fillColor;
    algo->draw(*circle, sw);
}

// ── Helper: draw the nose (mirrored V-shape) ──────────────────
// The sad nose has the V mirrored horizontally vs the happy nose
void SadSmileFace_DrawingAlgorithm::drawNose(SadSmileFace* face, double radius, ScreenWriter *sw) const {
    Point c = face->points[0]; // Face center

    Point p1(c.x,                c.y + radius / 8);   // Bottom tip of the nose (same as happy)
    Point p2(c.x + radius / 12,  c.y - radius / 16);  // Upper-RIGHT point (SWAPPED vs happy: left and right reversed)
    Point p3(c.x - radius / 12,  c.y - radius / 16);  // Upper-LEFT  point (gives the V a mirrored look)

    Line l1; // First line: p1 → p2
    l1.borderColor = face->borderColor;
    l1.addPoint(p1);
    l1.addPoint(p2);

    Line l2; // Second line: p2 → p3
    l2.borderColor = face->borderColor;
    l2.addPoint(p2);
    l2.addPoint(p3);

    DrawingAlgorithm *algo = new Line_Midpoint_DrawingAlgorithm();
    algo->draw(l1, sw); // Draw left arm of V
    algo->draw(l2, sw); // Draw right arm of V
}

// ── Helper: draw the mouth (sad/frowning arc) ─────────────────
// Same angular sweep as the happy mouth (200°–340°), but the
// y-formula is flipped: subtract abs(sin) instead of adding,
// and shift the whole arc down with + radius/1.4.
// This creates a downward curve (frown) instead of an upward curve (smile).
void SadSmileFace_DrawingAlgorithm::drawMouth(SadSmileFace* face, double radius, ScreenWriter *sw) const {
    sw->activate();

    for(double angle = 200.0; angle <= 340.0; angle += 0.1) { // Same angular range as happy face
        double rad = angle * M_PI / 180.0; // Degrees → radians

        double x = face->points[0].x + radius / 1.8 * cos(rad);          // Same x formula as happy face
        double y = face->points[0].y - radius / 2  * abs(sin(rad)) + radius / 1.4;
        //                            ↑ MINUS flips the arc (downward curve in screen coords)
        //                                                       ↑ +r/1.4 shifts the whole mouth downward so it sits below center
        sw->setPixel(round(x), round(y), face->borderColor);
    }

    sw->deactivate();
}

// ── Main assembly ─────────────────────────────────────────────
void SadSmileFace_DrawingAlgorithm::runAlgorithm(SadSmileFace* face, ScreenWriter *sw) const {
    Point center = face->points[0];
    Point edge   = face->points[1];
    double radius = face->points[0].euclideanDistance(face->points[1]); // Radius from Euclidean distance

    drawCircle(face, face->points[0], radius, sw); // 1. Face outline

    // 2. Left eye (same position as happy face)
    drawCircle(face, Point(center.x - radius / 3, center.y - radius / 4), radius / 8, sw);
    // 3. Right eye
    drawCircle(face, Point(center.x + radius / 3, center.y - radius / 4), radius / 8, sw);

    drawNose(face, radius, sw);  // 4. Mirrored nose
    drawMouth(face, radius, sw); // 5. Frowning mouth arc
}

// ── Public entry point ────────────────────────────────────────
void SadSmileFace_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_SAD_SMILE_FACE) {
        std::cerr << "SadSmileFace_DrawingAlgorithm::draw : shape is expected to be sad smile face" << std::endl;
        return;
    }
    if(inputShape.points.size() < 2) {
        std::cerr << "SadSmileFace_DrawingAlgorithm::draw : not enough points" << std::endl;
        return;
    }
    SadSmileFace* face = dynamic_cast<SadSmileFace*>(inputShape.clone()); // Clone + cast
    this->runAlgorithm(face, sw);
}
