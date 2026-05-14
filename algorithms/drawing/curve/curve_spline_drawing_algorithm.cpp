// ============================================================
// curve_spline_drawing_algorithm.cpp — Catmull-Rom / Hermite Spline
//
// CORE IDEA: A spline draws a smooth curve through a series of
// control points by fitting a Hermite cubic between every pair
// of consecutive points. A Hermite cubic is defined by:
//   • Two endpoints:  p0  and  p3
//   • Two tangents:   T0 (direction leaving p0)
//                     T1 (direction arriving at p3)
//
// The tangent at each interior point is estimated from its
// neighbours: T_i = C * (p_{i+1} - p_{i-1})
// where C (Cvalue) controls how "tight" or "loose" the curve is.
// This is the Catmull-Rom formula — a common spline used in
// animation, path smoothing, and font rendering.
//
// The Hermite basis functions h0–h3 blend the four inputs:
//   h0(t) = 2t³ - 3t² + 1   ← weight of start point p0
//   h1(t) = t³  - 2t² + t   ← weight of start tangent T0
//   h2(t) = -2t³ + 3t²      ← weight of end point p3
//   h3(t) = t³  - t²        ← weight of end tangent T1
//
// We sample t from 0 to 1 in steps of 0.001 (1000 pixels per
// segment) to get a smooth result.
// ============================================================

#include "curve_spline_drawing_algorithm.h" // Declares this class (CurveShape type, etc.)

// Constructor
// Cvalue is passed in as an integer percentage (e.g. 50 = 0.5).
// Dividing by 100 converts it to a 0–1 scale used in the tangent formula.
Curve_Spline_DrawingAlgorithm::Curve_Spline_DrawingAlgorithm(int Cvalue) : DrawingAlgorithm("Curve_Spline_DrawingAlgorithm") {
	this->Cvalue = Cvalue / 100.0; // Store as a fraction: e.g. 50 → 0.5. Higher = looser curves.
}

// ── Draw one Hermite segment from p0 to p3 ────────────────────
// t0 = tangent vector at p0 (how fast/direction the curve leaves p0)
// t1 = tangent vector at p3 (how fast/direction the curve arrives at p3)
// p0 = start endpoint of this segment
// p3 = end   endpoint of this segment
void Curve_Spline_DrawingAlgorithm::drawCurveHermite(Point t0, Point t1, Point p0, Point p3, ScreenWriter *sw, CurveShape* curve) const {
	double xt; // x coordinate of the current point on the curve
	double yt; // y coordinate of the current point on the curve

	sw->activate(); // Acquire Windows DC before writing pixels

	// Sample the curve at 1001 values of t from 0.0 to 1.0
	for (double t = 0; t <= 1; t += 0.001) {

		// ── Hermite basis functions ─────────────────────────────
		// These are cubic polynomials in t that act as "blend weights".
		// They are derived from the condition that the curve passes
		// exactly through p0 at t=0 and p3 at t=1, with matching tangents.

		double h0 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1; // Weight for p0: starts at 1, ends at 0
		double h1 = pow(t, 3) - 2 * pow(t, 2) + t;      // Weight for T0: starts at 0, ends at 0
		double h2 = -2 * pow(t, 3) + 3 * pow(t, 2);     // Weight for p3: starts at 0, ends at 1
		double h3 = pow(t, 3) - pow(t, 2);               // Weight for T1: starts at 0, ends at 0

		// ── Blend the four inputs to get the point at parameter t ──
		// Each coordinate is a weighted sum of the two endpoints and two tangents.
		xt = h0 * p0.x + h1 * t0.x + h2 * p3.x + h3 * t1.x; // x = blend of start_x, tangent0_x, end_x, tangent1_x
		yt = h0 * p0.y + h1 * t0.y + h2 * p3.y + h3 * t1.y; // y = blend of start_y, tangent0_y, end_y, tangent1_y

		sw->setPixel(round(xt), round(yt), curve->borderColor); // round() maps float → nearest integer pixel
	}

	sw->deactivate(); // Release Windows DC
}

// ── Run the full spline across all user control points ────────
void Curve_Spline_DrawingAlgorithm::runAlgorithm(CurveShape* curve, ScreenWriter *sw) const {
	double c1 = Cvalue; // Local copy of the tension constant (0 = tight/straight, 1 = very loose)

	// Compute the tangent at the FIRST interior point (index 1) using its neighbours (0 and 2)
	// Formula: T_i = C * (p_{i+1} - p_{i-1})  ← Catmull-Rom tangent estimate
	Point T0;
	T0.x = c1 * (curve->points[2].x - curve->points[0].x); // x component of the outgoing tangent at points[1]
	T0.y = c1 * (curve->points[2].y - curve->points[0].y); // y component of the outgoing tangent at points[1]

	// Walk through all interior segments: segment i connects points[i-1] to points[i]
	for (int i = 2; i < curve->points.size() - 1; i++) {
		// Compute the tangent at points[i] using its neighbours points[i-1] and points[i+1]
		Point T1;
		T1.x = c1 * (curve->points[i + 1].x - curve->points[i - 1].x); // Catmull-Rom tangent x at points[i]
		T1.y = c1 * (curve->points[i + 1].y - curve->points[i - 1].y); // Catmull-Rom tangent y at points[i]

		// Draw the Hermite cubic segment from points[i-1] to points[i],
		// with incoming tangent T0 (from the previous step) and outgoing tangent T1
		drawCurveHermite(T0, T1, curve->points[i - 1], curve->points[i], sw, curve);

		T0 = T1; // The outgoing tangent at points[i] becomes the incoming tangent for the next segment
	}
}

// ── Public entry point ─────────────────────────────────────────
void Curve_Spline_DrawingAlgorithm::draw(const Shape &inputShape, ScreenWriter *sw) const {
	if(inputShape.getType() != SHAPE_CURVE) { // Guard: only accept Curve shapes
		std::cerr << "Curve_Spline_DrawingAlgorithm::draw : shape to draw must be Curve" << std::endl;
		return;
	}
	if(inputShape.getSize() < 4) { // Need at least 4 points: 2 endpoints + 2 for tangent estimation
		std::cerr << "Curve spline requires at least 4 points" << std::endl;
		return;
	}
	CurveShape* curve = dynamic_cast<CurveShape*>(inputShape.clone()); // Clone + downcast to typed CurveShape*
	this->runAlgorithm(curve, sw);
}
