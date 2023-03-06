#include "vector.h"

namespace math {
    
    vec& vec::set(int index, double value) {
        set(index, 0, value);
        return *this;
    }

    vec& vec::set(const std::vector<double>& list) {
        setColumn(0, list);
        return *this;
    }

    double vec::get(int index) const {
        return get(index, 0);
    }

    int vec::getDimension() const {
        return getAmountOfRows();
    }

    //> Operations
    double vec::length() const {
        double out = 0;
        for(int i = 0; i < getDimension(); i++) {
            out += get(i) * get(i);
        }
        return std::sqrt(out);
    }

    double vec::dot(const vec& v2) const {
        if(sameDimensions(v2)) return 0;

        double out = 0;
        for(int i = 0; i < getDimension(); i++) {
            out += get(i) * v2.get(i);
        }

        return out;
    }

    vec vec::cross(const vec& v2) const {
        if(!(sameDimensions(v2) && getDimension() == 3)) return vec{ getDimension() };
        return vec3d(
            get(1) * v2.get(2) - get(2) * v2.get(1),
            get(2) * v2.get(0) - get(0) * v2.get(2),
            get(0) * v2.get(1) - get(1) * v2.get(0)
        );
    }

    vec& vec::normalize(double wantedLength) {
        double length = (*this).length();
        if(length == 0) {
            for(int i = 0; i < getDimension(); i++) {
                set(i, 0);
            }
        } else {
            *this /= length;
        }

        *this *= wantedLength;
        return *this;
    }

    vec vec::normalize(double wantedLength) const {
        vec out{ *this };
        out.normalize();
        return out;
    }

    double vec::distanceTo(const vec& v2) const {
        return (v2 - *this).length();
    }

    //> Export
    std::vector<double> vec::toList() {
        std::vector<double> out{};
        return toList(out);
    }
    
    std::vector<double>& vec::toList(std::vector<double>& in) {
        for(int i = 0; i < getDimension(); i++) {
            in.push_back(get(i));
        }
        return in;
    }

    vec operator+(const vec& v1, const vec& v2) {
        matrix result = (matrix) v1 + (matrix) v2;
        return result.getColumn(0);
    }

    vec operator-(const vec& v1, const vec& v2) {
        matrix result = (matrix) v1 - (matrix) v2;
        return result.getColumn(0);
    }
    vec operator*(const vec& v, float val) {
        matrix result = (matrix) v * val;
        return result.getColumn(0);
    }

    vec operator*(float val, const vec& v) {
        matrix result = (matrix) v * val;
        return result.getColumn(0);
    }

    vec operator/(const vec& v, float val) {
        matrix result = (matrix) v / val;
        return result.getColumn(0);
    }

    vec operator/(float val, const vec& v) {
        matrix result = (matrix) v / val;
        return result.getColumn(0);
    }

    std::ostream& operator<< (std::ostream& out, const vec& m) {
        out << "( ";
        for(int i = 0; i < m.getDimension(); i++) {
            out << m.get(i);
            if(i < m.getDimension() - 1) out << " | ";
        }
        out << " )";
        return out;
    }

    vec operator-(const vec& v1) {
        matrix result = -(matrix) v1;
        return result.getColumn(0);
    }
}
