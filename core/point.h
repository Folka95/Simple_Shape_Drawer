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
    double dx(const Point& other) {
        return abs(x - other.x);
    }

    double dy(const Point& other) {
        return abs(y - other.y);
    }

    double slope(const Point& other) {
        return (double)dy(other) / (double)dx(other);
    }

    // Distance
    long long manhattanDistance(const Point& other) {
        return (long long)dx(other) + (long long)dy(other);
    }

    double euclideanDistance(const Point& other) {
        return sqrt(dx(other) * dx(other) + dy(other) * dy(other));
    }

    // Operators
    Point operator+(const Point& other) {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(const Point& other) {
        return Point(x * other.x, y * other.y);
    }

    Point operator*(double factor) {
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
};

#endif //POINT_H
