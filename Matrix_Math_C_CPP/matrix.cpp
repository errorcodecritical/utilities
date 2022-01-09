#include "matrix.hpp"

matrix::matrix(unsigned int rows, unsigned int columns) {
    this->rows = rows; this->columns = columns;
    elements = (float*)calloc(rows * columns, sizeof(float));
};

matrix::~matrix() {
    free(elements);
};

float* matrix::operator[] (unsigned int row) {
    //assert row < rows
    return &elements[row * columns];
}

matrix matrix::row(unsigned int index) {
    matrix result(1, columns);

    for (int element = 0; element < columns; element++) {
        result.elements[element] = elements[index * columns + element];
    }

    return result;
}

matrix matrix::column(unsigned int index) {
    matrix result(rows, 1);

    for (int element = 0; element < rows; element++) {
        result.elements[element] = elements[element * rows + index];
    }
    
    return result;
}

void matrix::operator= (const float (&set)[]) {
    for (int element = 0; element < rows * columns; element++) {
        elements[element] = set[element];
    }
}

matrix matrix::operator+ (matrix operand) {
    matrix result(rows, columns);

    if ((rows != operand.rows) || (columns != operand.columns)) {
        //Size mismatch!
        return result;
    }

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < operand.columns; column++) {
            result.elements[row * columns + column] = 
                elements[row * columns + column] + 
                operand.elements[row * columns + column];
        }
    }
    return result;
}

matrix matrix::operator- () {
    matrix result(rows, columns);

    for (int element = 0; element < rows * columns; element++) {
        result.elements[element] = -elements[element];
    }

    return result;
}

matrix matrix::operator- (matrix &operand) {
    matrix result(rows, columns);

    if ((rows != operand.rows) || (columns != operand.columns)) {
        //Size mismatch!
        return result;
    }

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < operand.columns; column++) {
            result.elements[row * columns + column] = 
                elements[row * columns + column] - 
                operand.elements[row * columns + column];
        }
    }
    return result;
}

matrix matrix::operator* (float operand) {
    matrix result(rows, columns);

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            result.elements[row * columns + column] = 
                elements[row * columns + column] * operand;
        }
    }
    return result;
}

matrix matrix::operator* (matrix operand) {
    matrix result(rows, operand.columns);

    if (columns != operand.rows) {
        //Size mismatch!
        return result;
    }

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < operand.columns; column++) {
            for (int element = 0; element < columns; element++) {
                float a = elements[row * columns + element];
                float b = operand.elements[element * operand.columns + column];

                result[row][column] += a * b;
            }
        }
    }
    return result;
}

matrix matrix::transpose() {
    matrix result(columns, rows);

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            result.elements[column * rows + row] = 
                elements[row * columns + column];
        }
    }     

    return result;   
}

float matrix::determinant() {
    float result = 0;

    if (rows != columns) {
        // Size mismatch, must be square!
        return result;
    }

    if (rows == 1) {
        result = elements[0];
    } else if (rows == 2) {
        result = elements[0] * elements[3] - elements[1] * elements[2];
    } else {
        for (int i = 0, multiplier = 1; i < columns; i++) {
            matrix minor(rows - 1, columns - 1);

            for (int row = 1, r = 0; row < rows; row++) {
                for (int column = 0, c = 0; column < columns; column++) {
                    if (column == i) {
                        continue;
                    }
                    minor[r][c] = elements[row * columns + column];
                    c++;
                }
                r++;
            }
            result += multiplier * elements[i] * minor.determinant();
            multiplier *= -1;
        }
    }
    return result;
}

matrix matrix::minor() {
    matrix result(rows, columns);

    if (rows != columns) {
        // Size mismatch, must be square!
        return result;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix minor(rows - 1, columns - 1);

            for (int row = 0, r = 0; row < rows; row++) {
                if (row == i) {
                    continue;
                }
                for (int column = 0, c = 0; column < columns; column++) {
                    if (column == j) {
                        continue;
                    }
                    minor[r][c] = elements[row * columns + column];
                    c++;
                }
                r++;
            }
            result[i][j] = minor.determinant();
        }
    }
    return result;
}

matrix matrix::inverse() {
    matrix result(rows, columns);
    float det = determinant();

    if (det == 0) {
        // Matrix is non-invertible
        return result;
    }

    matrix min = minor();

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            min[row][column] *= ((row + column) % 2 == 0) ? 1 : -1;
        }
    }

    matrix adjoint = min.transpose();

    result = adjoint * (1 / det);

    return result;
}