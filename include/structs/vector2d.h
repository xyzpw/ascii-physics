#pragma once

struct Vector2D {
    double x;
    double y;

    Vector2D(double _x, double _y) : x(_x), y(_y){};

    double getMagnitude();

    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D& operator+=(const Vector2D& other){
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(double scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }
};
