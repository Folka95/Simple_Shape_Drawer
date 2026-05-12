#include "curve_spline_drawing_algorithm.h"


Curve_Spline_DrawingAlgorithm::Curve_Spline_DrawingAlgorithm(int Cvalue) : DrawingAlgorithm() {
	this->Cvalue = Cvalue / 100.0;
}

void Curve_Spline_DrawingAlgorithm::drawCurveHermite(Point t0, Point t1, Point p0, Point p3, ScreenWriter *sw, CurveShape* curve) const {
	double xt;
	double yt;
	sw->activate();
	for (double t = 0; t <= 1; t += 0.001) {
		double h0 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
		double h1 = pow(t, 3) - 2 * pow(t, 2) + t;
		double h2 = -2 * pow(t, 3) + 3 * pow(t, 2);
		double h3 = pow(t, 3) - pow(t, 2);
		xt = h0 * p0.x + h1 * t0.x + h2 * p3.x + h3 * t1.x;
		yt = h0 * p0.y + h1 * t0.y + h2 * p3.y + h3 * t1.y;
		sw->setPixel(round(xt), round(yt), curve->borderColor);
	}
	sw->deactivate();
}

void Curve_Spline_DrawingAlgorithm::runAlgorithm(CurveShape* curve, ScreenWriter *sw) const {
	double c1 = Cvalue;
	Point T0;
	T0.x = c1 * (curve->points[2].x - curve->points[0].x);
	T0.y = c1 * (curve->points[2].y - curve->points[0].y);
	for (int i = 2; i < curve->points.size() - 1; i++) {
		Point T1;
		T1.x = c1 * (curve->points[i + 1].x - curve->points[i - 1].x);
		T1.y = c1 * (curve->points[i + 1].y - curve->points[i - 1].y);
		drawCurveHermite(T0,T1,curve->points[i - 1],curve->points[i],sw, curve);
		T0 = T1;
	}
}

void Curve_Spline_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
	if(inputShape.getType() != SHAPE_CURVE) {
		std::cerr << "Curve_Spline_DrawingAlgorithm::draw : shape to draw must be Curve" << std::endl;
		return;
	}
	if(inputShape.getSize() < 4) {
		std::cerr << "Curve spline requires at least 4 points" << std::endl;
		return;
	}
	CurveShape* curve = dynamic_cast<CurveShape*>(inputShape.clone());
	this->runAlgorithm(curve, sw);
}