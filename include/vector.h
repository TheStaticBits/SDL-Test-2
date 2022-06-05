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

    // Conditionals
    bool operator==(const Vect<T>& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vect<T>& other) const { return x != other.x || y != other.y; }

    bool operator<(const Vect<T>& other) const { return x < other.x && y < other.y; }
    bool operator>(const Vect<T>& other) const { return x > other.x && y > other.y; }
    
    bool operator<=(const Vect<T>& other) const { return x <= other.x && y <= other.y; }
    bool operator>=(const Vect<T>& other) const { return x >= other.x && y >= other.y; }

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

    template <typename U>
    Vect<U> cast() const { return Vect<U>(static_cast<U>(x),
                                          static_cast<U>(y)); }

    template <typename U>
    inline U xCast() const { return static_cast<U>(x); }
    template <typename U>
    inline U yCast() const { return static_cast<U>(y); }

    inline void lock(Vect<T> max, Vect<T> min) {
        if (x < min.x) x = min.x; else if (x > max.x) x = max.x; 
        if (y < min.y) y = min.y; else if (y > max.y) y = max.y; 
    }

    inline void print() const
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};