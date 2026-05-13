#include "happy_smile_face_drawing_algorithm.h"

#include <cmath>
#include "../circle/circle_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/circle.h"
#include "../../../core/shapes/curve.h"
#include "../../../core/shapes/line.h"
#include "../curve/curve_spline_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"

HappySmileFace_DrawingAlgorithm::HappySmileFace_DrawingAlgorithm()
    : DrawingAlgorithm("HappySmileFace_DrawingAlgorithm") {
}

void HappySmileFace_DrawingAlgorithm::drawCircle(HappySmileFace* face, const Point& center, double radius, ScreenWriter *sw) const {
    Shape *circle = new Circle();
    circle->addPoint(center);
    circle->addPoint(Point(center.x + radius, center.y));
    DrawingAlgorithm *algo = new Circle_MidPoint_DrawingAlgorithm();
    circle->borderColor = face->borderColor;
    circle->fillColor = face->fillColor;
    algo->draw(*circle, sw);
}

void HappySmileFace_DrawingAlgorithm::drawNose(HappySmileFace* face, double radius, ScreenWriter *sw) const {
    Point c = face->points[0];

    Point p1(c.x, c.y + radius / 8);
    Point p2(c.x - radius / 12, c.y - radius / 16);
    Point p3(c.x + radius / 12, c.y - radius / 16);

    Line l1;
    l1.borderColor = face->borderColor;
    l1.addPoint(p1);
    l1.addPoint(p2);

    Line l2;
    l2.borderColor = face->borderColor;
    l2.addPoint(p2);
    l2.addPoint(p3);

    DrawingAlgorithm *algo = new Line_Midpoint_DrawingAlgorithm();
    algo->draw(l1, sw);
    algo->draw(l2, sw);
}

void HappySmileFace_DrawingAlgorithm::drawMouth(HappySmileFace* face, double radius, ScreenWriter *sw) const {
    sw->activate();
    for(double angle = 200.0; angle <= 340.0; angle += 0.1) {
        double rad = angle * M_PI / 180.0;
        double x = face->points[0].x + radius / 1.8 * cos(rad);
        double y = face->points[0].y + radius / 2 * abs(sin(rad));
        sw->setPixel(round(x), round(y), face->borderColor);
    }
    sw->deactivate();
}

void HappySmileFace_DrawingAlgorithm::runAlgorithm(HappySmileFace* face, ScreenWriter *sw) const {
    Point center = face->points[0];
    Point edge = face->points[1];
    double radius = face->points[0].euclideanDistance(face->points[1]);

    drawCircle(face, face->points[0], radius, sw);

    drawCircle(face, Point(center.x - radius / 3, center.y - radius / 4), radius / 8, sw);
    drawCircle(face, Point(center.x + radius / 3, center.y - radius / 4), radius / 8, sw);

    drawNose(face, radius, sw);

    drawMouth(face, radius, sw);
}

void HappySmileFace_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
    if(inputShape.getType() != SHAPE_HAPPY_SMILE_FACE) {
        std::cerr
            << "HappySmileFace_DrawingAlgorithm::draw : shape is expected to be happy smile face" << std::endl;
        return;
    }
    if(inputShape.points.size() < 2) {
        std::cerr
            << "HappySmileFace_DrawingAlgorithm::draw : not enough points" << std::endl;
        return;
    }
    HappySmileFace* face = dynamic_cast<HappySmileFace*>(inputShape.clone());
    this->runAlgorithm(face, sw);
}
