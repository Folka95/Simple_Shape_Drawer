#include "curve_spline_drawing_algorithm.h"
#include <bits/stdc++.h>

Curve_Spline_DrawingAlgorithm::Curve_Spline_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Curve_Spline_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CURVE) {
        std::cerr << "Curve_Spline_DrawingAlgorithm::draw : shape to draw must be Curve" << std::endl;
        return;
    }

    sw->activate();


    sw->deactivate();
}