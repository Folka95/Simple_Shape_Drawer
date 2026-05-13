#pragma once
#ifndef POINT_H
#define POINT_H
#include <cmath>

class Point {
public:
    double x, y;

    // Constructors
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    // Destructor
    ~Point() {}

    // Differential
    double dx(const Point& other) const {
        return other.x - x;
    }

    double dy(const Point& other) const {
        return other.y - y;
    }

    double slope(const Point& other) const {
        if(dx(other) == 0) {
            return std::numeric_limits<double>::infinity();;
        }
        return dy(other) / dx(other);
    }

    // Distance
    long long manhattanDistance(const Point& other) const {
        return (long long)dx(other) + (long long)dy(other);
    }

    double euclideanDistance(const Point& other) const {
        return sqrt(dx(other) * dx(other) + dy(other) * dy(other));
    }

    // Linear Interpolation
    Point lerp(const Point& b, double t) const {
        return *this + (b - *this) * t;
    }

    // Operators
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(const Point& other) const {
        return Point(x * other.x, y * other.y);
    }

    Point operator*(double factor) const {
        return Point(x * factor, y * factor);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point& operator*=(const Point& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Point& operator*=(double factor) {
        x *= factor;
        y *= factor;
        return *this;
    }

    bool operator<(const Point& other) const {
        double angle1 = atan2(y, x);
        double angle2 = atan2(other.y, other.x);
        return angle1 > angle2;
    }
};

#endif //POINT_H
