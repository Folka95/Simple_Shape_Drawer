#include "happy_smile_face_drawing_algorithm.h"

#include <cmath>
#include "../circle/circle_midpoint_drawing_algorithm.h"
#include "../../../core/shapes/circle.h"
#include "../../../core/shapes/curve.h"
#include "../../../core/shapes/line.h"
#include "../curve/curve_spline_drawing_algorithm.h"
#include "../line/line_midpoint_drawing_algorithm.h"

HappySmileFace_DrawingAlgorithm::HappySmileFace_DrawingAlgorithm()
    : DrawingAlgorithm() {
}

void HappySmileFace_DrawingAlgorithm::drawCircle(const Shape &face, const Point& center, double radius, ScreenWriter *sw) const {
    Shape *circle = new Circle();
    circle->addPoint(center);
    circle->addPoint(Point(center.x + radius, center.y));
    DrawingAlgorithm *algo = new Circle_MidPoint_DrawingAlgorithm();
    circle->borderColor = face.borderColor;
    circle->fillColor = face.fillColor;
    algo->draw(*circle, sw);
}

void HappySmileFace_DrawingAlgorithm::drawNose(const Shape &face, double radius, ScreenWriter *sw) const {
    Point c = face.points[0];

    Point p1(c.x, c.y + radius / 8);
    Point p2(c.x - radius / 12, c.y - radius / 16);
    Point p3(c.x + radius / 12, c.y - radius / 16);

    Line l1;
    l1.borderColor = face.borderColor;
    l1.addPoint(p1);
    l1.addPoint(p2);

    Line l2;
    l2.borderColor = face.borderColor;
    l2.addPoint(p2);
    l2.addPoint(p3);

    DrawingAlgorithm *algo = new Line_Midpoint_DrawingAlgorithm();
    algo->draw(l1, sw);
    algo->draw(l2, sw);
}

void HappySmileFace_DrawingAlgorithm::drawMouth(const Shape &face, double radius, ScreenWriter *sw) const {
    sw->activate();
    for(double angle = 200.0; angle <= 340.0; angle += 0.1) {
        double rad = angle * M_PI / 180.0;
        double x = face.points[0].x + radius / 1.8 * cos(rad);
        double y = face.points[0].y + radius / 2 * abs(sin(rad));
        sw->setPixel(round(x), round(y), face.borderColor);
    }
    sw->deactivate();
}

void HappySmileFace_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_HAPPY_SMILE_FACE) {
        std::cerr
            << "HappySmileFace_DrawingAlgorithm::draw : shape is expected to be happy smile face" << std::endl;
        return;
    }
    if(shape.points.size() < 2) {
        std::cerr
            << "HappySmileFace_DrawingAlgorithm::draw : not enough points" << std::endl;
        return;
    }

    Point center = shape.points[0];
    Point edge = shape.points[1];
    double radius = shape.points[0].euclideanDistance(shape.points[1]);

    drawCircle(shape, shape.points[0], radius, sw);

    drawCircle(shape, Point(center.x - radius / 3, center.y - radius / 4), radius / 8, sw);
    drawCircle(shape, Point(center.x + radius / 3, center.y - radius / 4), radius / 8, sw);

    drawNose(shape, radius, sw);

    drawMouth(shape, radius, sw);
}