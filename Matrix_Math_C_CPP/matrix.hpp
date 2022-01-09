#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

class matrix { // Row-Major
private:
    float* elements;
public:
    unsigned int rows = 0, columns = 0;

    matrix(unsigned int rows = 1, unsigned int columns = 1);

    ~matrix();

    float* operator[] (unsigned int row);

    matrix row(unsigned int index);

    matrix column(unsigned int index);

    void operator= (const float (&set)[]);

    matrix operator+ (matrix operand);

    matrix operator- (matrix &operand);
    
    matrix operator- ();

    matrix operator* (float operand);

    matrix operator* (matrix operand);

    matrix transpose();

    float determinant();

    matrix minor();

    matrix inverse();
};

#endif