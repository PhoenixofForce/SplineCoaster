#include "matrix.h"
#include "vector.h"

namespace math {
   
    static matrix identity(int rank) {
        matrix out{rank, rank};
        for(int i = 0; i < rank; i++) out.set(i, i, 1);
        return out;
    }

    static matrix rot2D(double angleInRadians) {
        matrix out{2, 2};
        out
          .setRow(0, { cos(angleInRadians), -sin(angleInRadians)})
          .setRow(1, { sin(angleInRadians), cos(angleInRadians)});
          
        return out;
    }

    static matrix rot3D(double x, double y, double z) {
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

    matrix& matrix::set(int rowIndex, int columnIndex, double value) {
        if(positionOutOfBounds(rowIndex, columnIndex)) {
            std::cout << "ERROR: " << "Position out of bounds (" << rowIndex << " | " << columnIndex << ")"
                      << " >> (" << amountOfRows << " | " << amountOfColumns << ")" << std::endl; 
            return *this;
        }

        matrixRepresentation[{rowIndex, columnIndex}] = value;

        return *this;
    }

    matrix& matrix::setRow(int rowIndex, const std::vector<double>& list) {
        for(int c = 0; c < std::min(amountOfColumns, (int) list.size()); c++) {
            set(rowIndex, c, list.at(c));
        }

        return *this;
    }

    matrix& matrix::setColumn(int columnIndex, const std::vector<double>& list) {
        for(int r = 0; r < std::min(amountOfRows, (int) list.size()); r++) {
            set(r, columnIndex, list.at(r));
        }

        return *this;
    }

    //> Getter

    double matrix::get(int rowIndex, int columnIndex) const {
        return (*this)(rowIndex, columnIndex);
    }

    double matrix::operator()(int rowIndex, int columnIndex) const {
        if(positionOutOfBounds(rowIndex, columnIndex)) {
            std::cout << "ERROR: "<< "Position out of bounds (" << rowIndex << " | " << columnIndex << ")"
                      << " >> (" << amountOfRows << " | " << amountOfColumns << ")" << std::endl; 
            return 0;
        }

        std::pair<int, int> index{ rowIndex, columnIndex };
        if(matrixRepresentation.find(index) != matrixRepresentation.end()) {
            return matrixRepresentation.at(index);
        }

        return 0;
    }

    vec matrix::getColumn(int columnIndex) {
        vec out{ getAmountOfRows() };
        if(positionOutOfBounds(0, columnIndex)) return out;

        for(int r = 0; r < getAmountOfRows(); r++) {
            out.set(r, get(r, columnIndex));
        }

        return out;
    }

    vec matrix::getRow(int rowIndex) {
        vec out{ getAmountOfColumns() };
        if(positionOutOfBounds(rowIndex, 0)) return out;

        for(int c = 0; c < getAmountOfColumns(); c++) {
            out.set(c, get(rowIndex, c));
        }

        return out;
    }

    //> Properties

    int matrix::getAmountOfRows() const {
        return amountOfRows;
    }

    int matrix::getAmountOfColumns() const {
        return amountOfColumns;
    }

    bool matrix::positionOutOfBounds(int rowIndex, int columnIndex) const {
        return rowIndex >= amountOfRows || columnIndex >= amountOfColumns
            || rowIndex < 0 || columnIndex < 0; 
    }

    bool matrix::sameDimensions(const matrix& otherMatrix) const {
        return getAmountOfRows() == otherMatrix.getAmountOfRows() && getAmountOfColumns() == otherMatrix.getAmountOfColumns();
    }

    //> Operations

    double matrix::det() {
        if(getAmountOfColumns() != getAmountOfRows()) {
            return 0;
        }

        double out = 0;
        for(int currentColumn = 0; currentColumn < getAmountOfColumns(); currentColumn++) {
            out += get(0, currentColumn) * get(1, (currentColumn + 1                       ) % getAmountOfColumns()) * get(1, (currentColumn + 2                       ) % getAmountOfColumns());
            out -= get(0, currentColumn) * get(1, (currentColumn - 1 + getAmountOfColumns()) % getAmountOfColumns()) * get(1, (currentColumn - 2 + getAmountOfColumns()) % getAmountOfColumns());
        }

        return out;
    }

    matrix matrix::transpose() {
        matrix out{ getAmountOfColumns(), getAmountOfRows() };

        for(int r = 0; r < getAmountOfRows(); r++) {
            for(int c = 0; c < getAmountOfColumns(); c++) {
                out.set(c, r, get(r, c));
            }
        }

        return out;
    }

    matrix matrix::submatrix(const std::vector<int>& deletedRows, const std::vector<int>& deleteColumns) {
        matrix out{ getAmountOfRows() - (int) deletedRows.size(), getAmountOfColumns() - (int) deleteColumns.size() };

        int rowOffset = 0;
        for(int r = 0; r < getAmountOfRows(); r++) {
            if(std::find(deletedRows.begin(), deletedRows.end(), r) != deletedRows.end()) {
                rowOffset++;
                continue;
            }

            int columnOffset = 0;
            for(int c = 0; c < getAmountOfColumns(); c++) {
                if(std::find(deleteColumns.begin(), deleteColumns.end(), c) != deleteColumns.end()) {
                    columnOffset++;
                    continue;
                }

                out.set(r - rowOffset, c - columnOffset, get(r, c));
            }
        }

        return out;
    }

    matrix matrix::adjoint() {
        if(getAmountOfColumns() != getAmountOfRows()) {
            std::cerr << "Matrix is not square, cannot calculate adjoint";
            int min  = std::min(getAmountOfColumns(), getAmountOfRows());
            return matrix{ min, min };
        }

        matrix out{ getAmountOfColumns(), getAmountOfColumns() };
        for(int rowIndex = 0; rowIndex < amountOfRows; rowIndex++) {
            for(int columnIndex = 0; columnIndex < amountOfColumns; columnIndex++) {
                matrix minor{ submatrix({ rowIndex }, { columnIndex }) };

                double sign = (rowIndex + columnIndex) % 2 == 0? 1: -1;
                out.set(rowIndex, columnIndex, sign * minor.det());
            }
        }

        return out;
    }

    matrix matrix::inverse() {
        double det = this->det();
        if(det == 0) {
            std::cerr << "Det is equal to zero, cannot calculate inverse";
            return matrix{ amountOfColumns, amountOfColumns };
        }
        
        return det * adjoint();
    }

    //more operations

    matrix& matrix::operator+=(const matrix& otherMatrix) {
        if(!sameDimensions(otherMatrix)) {
            std::cout << "ERROR: Matrix ranks do not match" << std::endl; 
        }

        for(int r = 0; r < amountOfRows; r++) {
            for(int c = 0; c < amountOfColumns; c++) {
                set(r, c, get(r, c) + otherMatrix.get(r, c));
            }
        }

        return *this;
    }

    matrix& matrix::operator-=(const matrix& otherMatrix) {
        if(!sameDimensions(otherMatrix)) {
            std::cout << "ERROR: Matrix ranks do not match" << std::endl; 
        }

        for(int r = 0; r < amountOfRows; r++) {
            for(int c = 0; c < amountOfColumns; c++) {
                set(r, c, get(r, c) - otherMatrix.get(r, c));
            }
        }

        return *this;
    }

    matrix& matrix::operator*=(double value) {
        for(int r = 0; r < amountOfRows; r++) {
            for(int c = 0; c < amountOfColumns; c++) {
                set(r, c, get(r, c) * value);
            }
        }

        return *this;
    }

    matrix& matrix::operator/=(double value) {
        for(int r = 0; r < amountOfRows; r++) {
            for(int c = 0; c < amountOfColumns; c++) {
                set(r, c, get(r, c) / value);
            }
        }

        return *this;
    }

    matrix operator+(const matrix& v1, const matrix& v2) {
        matrix out{ v1.getAmountOfRows(), v1.getAmountOfColumns() };
        out += v1;
        out += v2;
        return out;
    }

    matrix operator-(const matrix& v1, const matrix& v2) {
        matrix out{ v1.getAmountOfRows(), v1.getAmountOfColumns() };
        out += v1;
        out -= v2;
        return out;
    }

    matrix operator*(const matrix& v1, const matrix& v2) {
        matrix out{ v1.getAmountOfRows(), v2.getAmountOfColumns() };
        if(v1.getAmountOfColumns() != v2.getAmountOfRows()) {
            std::cout << "ERROR: column rank does not match row rank";
            return out;
        }

        for(int r = 0; r < out.getAmountOfRows(); r++) {
            for(int c = 0; c < out.getAmountOfColumns(); c++) {
                double value = 0;
                for(int k = 0; k < v1.getAmountOfColumns(); k++) {
                    value += v1.get(r, k) * v2.get(k, c);
                } 
                out.set(r, c, value);
            }
        }

        return out;
    }

    vec operator*(const matrix& v1, const vec& v2) {
        return (v1 * (matrix) v2).getColumn(0);
    }

    matrix operator*(const matrix& v1, float val) {
        matrix out{ v1.getAmountOfRows(), v1.getAmountOfColumns() };
        out += v1;
        out *= val;
        return out;
    }

    matrix operator*(float val, const matrix& v) {
        return v * val;
    }

    matrix operator/(const matrix& v1, float val) {
        matrix out{ v1.getAmountOfRows(), v1.getAmountOfColumns() };
        out += v1;
        out /= val;
        return out;
    }

    matrix operator/(float val, const matrix& v) {
        return v * val;
    }

    std::ostream& operator<< (std::ostream& outPrint, const matrix& out) {
        outPrint << "[ " << out.getAmountOfRows() << "x" << out.getAmountOfColumns() << std::endl;
        for(int r = 0; r < out.getAmountOfRows(); r++) {
            for(int c = 0; c < out.getAmountOfColumns(); c++) {
                outPrint << "\t" << out.get(r, c);
                if(c < out.getAmountOfColumns() - 1) outPrint << "\t";
            }
            outPrint << std::endl;
        }
        outPrint << "]";

         return outPrint;
    }

    matrix operator-(const matrix& v1) {
        matrix out{v1};
        out *= -1;
        return out;
    }

    bool operator== (const matrix& v1, const matrix& v2) {
        if(!v1.sameDimensions(v2)) return false;

        for(int r = 0; r < v1.getAmountOfRows(); r++) {
            for(int c  = 0; c < v1.getAmountOfColumns(); c++) {
                if(v1.get(r, c) != v2.get(r, c)) return false;
            }
        }

        return true;
    }

    bool operator!= (const matrix& v1, const matrix& v2) {
        return !(v1 == v2);
    }

    matrix lookRotation(const vec& inForward, const vec& up) {
        vec forward = vec{ inForward }.normalize();
        vec binormal = up.cross(forward).normalize();
        vec normal = forward.cross(binormal).normalize();

        return matrix{ 3, 3 }
            .setRow(0, binormal.toList())
            .setRow(1, normal.toList())
            .setRow(2, forward.toList());    
    }
}