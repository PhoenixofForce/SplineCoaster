#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"
#include "raylib.h"

namespace math {

    class vec : public matrix {
    public:

    //> Constructors
    vec(int dimension)
    : matrix{ dimension, 1}
    { }

    vec(const vec& m) 
    : matrix{ m }
    { }

    static vec vec2d(double x, double y) {
        return vec{x, y};
    }

    static vec vec3d(double x, double y, double z) {
        return vec{x, y, z};
    }

    static vec fromList(const std::vector<double>& list) {
        vec out{ (int) list.size() };
        out.set(list);
        return out;
    }

    //> Setters
    vec& set(int index, double value);
    vec& set(const std::vector<double>& list);

    //> Getters
    double get(int index) const;

    //> Properties
    int getDimension() const;

    //> Operations
    double length() const;
    double dot(const vec& v2) const;
    vec cross(const vec& v2) const;
    vec& normalize(double wantedLength = 1);
    vec normalize(double wantedLength = 1) const;

    double distanceTo(const vec& v2) const;

    //> Export
    std::vector<double> toList();
    std::vector<double>& toList(std::vector<double>& in);

    Vector2 toVector2() {
        return Vector2{ (float) get(0), (float) get(1) };
    }

    Vector3 toVector3() {
        return Vector3{ (float) get(0), (float) get(1), (float) get(2) };
    }

    private:
        vec(double x, double y)
        : vec(2)
        {
            setColumn(0, {x, y});
        }

        vec(double x, double y, double z)
        : vec(3)
        {
            setColumn(0, {x, y, z});
        }

        using matrix::set;
        using matrix::setRow;
        using matrix::setColumn;
        using matrix::get;
        using matrix::getAmountOfColumns;
        using matrix::getAmountOfRows;
        using matrix::getColumn;
    };

    vec operator+(const vec& v1, const vec& v2);
    vec operator-(const vec& v1, const vec& v2);
    vec operator*(const vec& v, float val);
    vec operator*(float val, const vec& v);

    vec operator/(const vec& v, float val);
    vec operator/(float val, const vec& v);
    
    std::ostream& operator<< (std::ostream& out, const vec& m);
    vec operator-(const vec& v1);
};

#endif