#pragma once

#include <iostream>

template <typename T>
struct Vect
{
    T x, y;

    Vect(T x, T y) : x(x), y(y) {} // Intializer
    Vect() : x(0), y(0) {} // Without parameters (set to zero)

    // Copy constructor
    Vect(const Vect<T>& other) { x = other.x; y = other.y; }
    // Copy assignment operator
    Vect<T>& operator=(const Vect<T>& other) { x = (T)other.x; y = (T)other.y; return *this; }

    // Equals
    bool operator==(const Vect<T>& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vect<T>& other) const { return x != other.x || y != other.y; }

    // Addition
    Vect<T> operator+(const Vect<T>& other) const { return Vect<T>(x + other.x, y + other.y); }
    Vect<T>& operator+=(const Vect<T>& other) { x += other.x; y += other.y; return *this; }
    Vect<T> operator+(const T& other) const { return Vect<T>(x + other, y + other); }
    Vect<T>& operator+=(const T& other) { x += other; y += other; return *this; }

    // Subtraction
    Vect<T> operator-(const Vect<T>& other) const { return Vect<T>(x - other.x, y - other.y); }
    Vect<T>& operator-=(const Vect<T>& other) { x -= other.x; y -= other.y; return *this; }
    Vect<T> operator-(const T& other) const { return Vect<T>(x - other, y - other); }
    Vect<T>& operator-=(const T& other) { x -= other; y -= other; return *this; }

    // Multiplication
    Vect<T> operator*(const Vect<T>& other) const { return Vect<T>(x * other.x, y * other.y); }
    Vect<T>& operator*=(const Vect<T>& other) { x *= other.x; y *= other.y; return *this; }
    Vect<T> operator*(const T& other) const { return Vect<T>(x * other, y * other); }
    Vect<T> operator*=(const T& other) { x *= other; y *= other; return *this; }

    // Division
    Vect<T> operator/(const Vect<T>& other) const { return Vect<T>(x / other.x, y / other.y); }
    Vect<T>& operator/=(const Vect<T>& other) { x /= other.x; y /= other.y; return *this; }
    Vect<T> operator/(const T& other) const { return Vect<T>(x / other, y / other); }
    Vect<T> operator/=(const T& other) { x /= other; y /= other; return *this; }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};