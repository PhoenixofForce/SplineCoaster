#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>

namespace math {
    class vec;
    class matrix;
    matrix operator*(const matrix& v1, const matrix& v2);
    
    class matrix {
    public:
    
    //> Constructors
    matrix(int rows, int columns) 
    :amountOfRows{ rows }, amountOfColumns{ columns }
    { }

    matrix(const matrix& m) 
    : amountOfRows{ m.amountOfRows }, amountOfColumns{ m.amountOfColumns }, matrixRepresentation{ m.matrixRepresentation }
    { }

    static matrix identity(int rank) {
        matrix out{rank, rank};
        for(int i = 0; i < rank; i++) out.set(i, i, 1);
        return out;
    }

    static matrix rot2d(double angleInRadians) {
        matrix out{2, 2};
        out
          .setRow(0, { cos(angleInRadians), -sin(angleInRadians)})
          .setRow(1, { sin(angleInRadians), cos(angleInRadians)});
          
        return out;
    }

    static matrix rot3d(double x, double y, double z) {
        matrix rotX{3, 3};
        rotX.setRow(0, { 1, 0, 0 })
            .setRow(1, { 0, cos(x), -sin(x) })
            .setRow(2, { 0, sin(x), cos(x) });

        matrix rotY{3, 3};
        rotY.setRow(0, { cos(y), 0, sin(y) })
            .setRow(1, { 0, 1, 0 })
            .setRow(2, { -sin(y), 0, cos(y) });

        matrix rotZ{3, 3};
        rotZ.setRow(0, { cos(z), -sin(z), 0 })
            .setRow(1, { sin(z), cos(z), 0 })
            .setRow(2, { 0, 0, 1 });

        return rotX * rotY * rotZ;
    }

    //> Setter
    matrix& set(int rowIndex, int columnIndex, double value);
    matrix& setRow(int rowIndex, const std::vector<double>& list);
    matrix& setColumn(int columnIndex, const std::vector<double>& list);

    //> Getter
    double get(int rowIndex, int columnIndex) const;
    double operator()(int rowIndex, int columnIndex = 0) const;

    vec getColumn(int columnIndex);
    vec getRow(int rowIndex);

    //> Properties
    int getAmountOfRows() const;
    int getAmountOfColumns() const;
    bool sameDimensions(const matrix& otherMatrix) const;

    //> Operators
    double det();
    matrix transpose();
    matrix submatrix(const std::vector<int>& deletedRows, const std::vector<int>& deleteColumns);
    matrix adjoint();
    matrix inverse();

    matrix& operator+=(const matrix& otherMatrix);
    matrix& operator-=(const matrix& otherMatrix);
    matrix& operator*=(double value);
    matrix& operator/=(double value);

    //> Export

    private:
        int amountOfRows;
        int amountOfColumns;
        std::map<std::pair<int, int>, double> matrixRepresentation{};

        bool positionOutOfBounds(int rowIndex, int columnIndex) const;
    };

    //> More Operators
    matrix operator+(const matrix& v1, const matrix& v2);
    matrix operator-(const matrix& v1, const matrix& v2);

    matrix operator*(const matrix& v1, const matrix& v2);
    vec operator*(const matrix& v1, const vec& v2);

    matrix operator*(const matrix& v, float val);
    matrix operator*(float val, const matrix& v);

    matrix operator/(const matrix& v, float val);
    matrix operator/(float val, const matrix& v);
    std::ostream& operator<< (std::ostream& out, const matrix& m);

    matrix operator-(const matrix& v1);
    bool operator== (const matrix& v1, const matrix& v2);
    bool operator!= (const matrix& v1, const matrix& v2);

    matrix lookRotation(const vec& inForward, const vec& up);
}

#endif