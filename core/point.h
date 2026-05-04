#pragma once
#ifndef POINT_H
#define POINT_H
#include <bits/stdc++.h>

template<typename T>
class Point {
public:
    T x, y;
    Point();
    Point(T x, T y);
    ~Point();
    T dx(const Point< T > &other);
    T dy(const Point< T > &other);
    double slope(const Point< T > &other);
    long long manhattanDistance(const Point< T > &other);
    double euclideanDistance(const Point< T > &other);
    Point operator+(const Point< T > &other);
    Point operator-(const Point< T > &other);
    Point operator*(const Point< T > &other);
    Point operator*(T factor);
    Point& operator+=(const Point< T > &other);
    Point& operator-=(const Point< T > &other);
    Point& operator*=(const Point< T > &other);
    Point& operator*=(T factor);
};

#endif //POINT_H
