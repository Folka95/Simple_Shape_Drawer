#include "square_line_clipping_algorithm.h"
#include "../../../core/shapes/line.h"
#include "../../drawing/line/line_midpoint_drawing_algorithm.h"
#include <algorithm>


Square_Line_ClippingAlgorithm::Square_Line_ClippingAlgorithm() : ClippingAlgorithm("Square_Line_ClippingAlgorithm") {}

union Square_Line_ClippingAlgorithm::OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

Square_Line_ClippingAlgorithm::OutCode Square_Line_ClippingAlgorithm::GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) const {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
    return out;
}

void Square_Line_ClippingAlgorithm::VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) const{
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void Square_Line_ClippingAlgorithm::HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) const{
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void Square_Line_ClippingAlgorithm::drawLine(const Point &p1, const Point &p2, COLORREF borderColor, ScreenWriter *sw) const {
    Line *line = new Line();
    line->borderColor = borderColor;
    line->addPoint(p1);
    line->addPoint(p2);
    DrawingAlgorithm *line_midpoint_drawing_algorithm = new Line_Midpoint_DrawingAlgorithm();
    line_midpoint_drawing_algorithm->draw(*line, sw);
}





void Square_Line_ClippingAlgorithm::runAlgorithm(Line* line, Square* square, ScreenWriter *sw) const {
    if (line == nullptr || square == nullptr || line->points.size() < 2) {
        return;
    }

    const int xleft = (std::min)(square->getBottomLeft().x, square->getBottomRight().x);
    const int xright = (std::max)(square->getBottomLeft().x, square->getBottomRight().x);
    const int ymin = (std::min)(square->getTopLeft().y, square->getBottomRight().y);
    const int ymax = (std::max)(square->getTopLeft().y, square->getBottomRight().y);

    double x1 = line->points[0].x, y1 = line->points[0].y, x2 = line->points[1].x, y2 = line->points[1].y;

    OutCode out1 = GetOutCode(x1, y1, xleft, ymin, xright, ymax);
    OutCode out2 = GetOutCode(x2, y2, xleft, ymin, xright, ymax);
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi;
        if (out1.All) {
            if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)HIntersect(x1, y1, x2, y2, ymin, &xi, &yi);
            else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ymax, &xi, &yi);
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ymin, xright, ymax);
        } else {
            if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)HIntersect(x1, y1, x2, y2, ymin, &xi, &yi);
            else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ymax, &xi, &yi);
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ymin, xright, ymax);
        }
    }

    if (!out1.All && !out2.All) {
        Point p1(x1, y1);
        Point p2(x2, y2);
        drawLine(p1, p2, line->borderColor, sw);
    }


}

void Square_Line_ClippingAlgorithm::clip(const Shape &inputShape, const Shape &inputRegion, ScreenWriter *sw) const {
    if (inputShape.getType() != SHAPE_LINE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : shape to draw must be Line" << std::endl;
        return;
    }

    if (inputRegion.getType() != SHAPE_SQUARE) {
        std::cerr << "Square_Line_ClippingAlgorithm::clip : Region must be a Square" << std::endl;
        return;
    }
    Line* line = dynamic_cast<Line*>(inputShape.clone());
    Square* square = dynamic_cast<Square*>(inputRegion.clone());
    this->runAlgorithm(line, square, sw);
}

