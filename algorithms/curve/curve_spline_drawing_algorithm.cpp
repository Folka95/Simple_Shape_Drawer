#include "curve_spline_drawing_algorithm.h"
#include <bits/stdc++.h>

Curve_Spline_DrawingAlgorithm::Curve_Spline_DrawingAlgorithm() : DrawingAlgorithm() {

}

void Curve_Spline_DrawingAlgorithm::drawCurveHermite(Point t0, Point t1, Point p0, Point p3, ScreenWriter *sw, const Shape &shape) const {
	double xt;
	double yt;
	for (double t = 0; t <= 1; t += 0.001) {
		double h0 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
		double h1 = pow(t, 3) - 2 * pow(t, 2) + t;
		double h2 = -2 * pow(t, 3) + 3 * pow(t, 2);
		double h3 = pow(t, 3) - pow(t, 2);

		xt = h0 * p0.x + h1 * t0.x + h2 * p3.x + h3 * t1.x;
		yt = h0 * p0.y + h1 * t0.y + h2 * p3.y + h3 * t1.y;

		sw->setPixel(round(xt), round(yt), shape.borderColor);
	}
}

void Curve_Spline_DrawingAlgorithm::draw(const Shape &shape, ScreenWriter *sw) const {
    if(shape.getType() != SHAPE_CURVE) {
        std::cerr << "Curve_Spline_DrawingAlgorithm::draw : shape to draw must be Curve" << std::endl;
        return;
    }

    double c1 = 1.0;

    sw->activate();

    // Calculate the initial tangent at P[1] using neighbors P[2] and P[0]
    Point T0;
    T0.x = c1 * (shape.points[2].x - shape.points[0].x);
    T0.y = c1 * (shape.points[2].y - shape.points[0].y);

    // Loop through the points starting from the second point
    for (int i = 2; i < shape.points.size(); i++) 
    {
        // Calculate the tangent at the end of the current segment (at P[i])
        // using neighbors P[i+1] and P[i-1]
        Point T1;
        T1.x = c1 * (shape.points[i + 1].x - shape.points[i - 1].x);
        T1.y = c1 * (shape.points[i + 1].y - shape.points[i - 1].y);

        // Draw the segment between P[i-1] and P[i] 
        // using the tangents T0 (at start) and T1 (at end)
        drawCurveHermite(T0, T1, shape.points[i - 1], shape.points[i], sw, shape);

        // The current end tangent becomes the start tangent for the next segment
        T0 = T1;
    }

    sw->deactivate();
}