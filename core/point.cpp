#include "point.h"

// ---------------- Constructors & Destructor ----------------
template<typename T>
Point< T >::Point() {
    this->x = 0;
    this->y = 0;
}

template<typename T>
Point< T >::Point(T _x, T _y) {
    this->x = _x;
    this->y = _y;
}

template<typename T>
Point< T >::~Point() {

}

// ---------------- Differential ----------------
template <typename T>
T Point< T >::dx(const Point< T > &other) {
    return abs(this.x - other.x);
}

template <typename T>
T Point< T >::dy(const Point< T > &other) {
    return abs(this.y - other.y);
}

template <typename T>
double Point< T >::slope(const Point< T > &other) {
    return (double)this->dy(other) / (double)this->dx(other);
}

// ---------------- Distance ----------------
template <typename T>
long long Point< T >::manhattanDistance(const Point< T > &other) {
    return (long long)this->dx(other) + (long long)this->dy(other);
}

template <typename T>
double Point< T >::euclideanDistance(const Point< T > &other) {
    return sqrt((double)this->dx(other) * this->dx(other) +
                  (double)this->dy(other) * this->dy(other));
}

// ---------------- Operators ----------------

template <typename T>
Point< T > Point< T >::operator+(const Point< T > &other) {
    return Point< T >(this.x + other.x, this.y + other.y);
}

template <typename T>
Point< T > Point< T >::operator-(const Point< T > &other) {
    return Point< T >(this.x + other.x, this.y + other.y);
}

template <typename T>
Point< T > Point< T >::operator*(const Point< T > &other) {
    return Point< T >(this.x * other.x, this.y * other.y);
}

template <typename T>
Point< T > Point< T >::operator*(T factor) {
    return Point< T >(this.x * factor, this.y * factor);
}

template <typename T>
Point< T >& Point< T >::operator+=(const Point< T > &other) {
    this.x += other.x;
    this.y += other.y;
    return *this;
}

template <typename T>
Point< T >& Point< T >::operator-=(const Point< T > &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template <typename T>
Point< T >& Point< T >::operator*=(T factor) {
    this.x *= factor;
    this.y *= factor;
    return *this;
}

template <typename T>
Point< T >& Point< T >::operator*=(const Point< T > &other) {
    this.x *= other.x;
    this.y *= other.y;
    return *this;
}
