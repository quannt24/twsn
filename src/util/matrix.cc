/*
 * matrix.cc
 *
 *  Created on: Oct 1, 2013
 *      Author: quannt
 */

#include "matrix.h"
#include <algorithm>
#include <iostream>

namespace twsn {

/*
 * Initialize matrix with size rows*cols and having initial setup initMat.
 * initMat can be MAT_TYPE_ZERO or MAT_TYPE_IDENTITY.
 * Note:
 * - Only square matrix could be identity matrix. Set initMat as MAT_TYPE_IDENTITY for a
 * non-square matrix have no effect and the matrix is still set to zero.
 * - This function should only be used in constructors. It will not dispose old allocated
 * memory and cause leaks if be called in normal function.
 */
void Matrix::init(int rows, int cols, MatrixType initMat)
{
    if (rows <= 0 && cols <= 0) throw INVALID_MATRIX_SIZE;

    this->rows = rows;
    this->cols = cols;

    cell = new double*[rows];
    int i, j;
    for (i = 0; i < rows; i++) {
        cell[i] = new double[cols];
        for (j = 0; j < cols; j++) {
            cell[i][j] = 0;
            if (initMat == MAT_TYPE_IDENTITY && rows == cols && i == j) cell[i][j] = 1;
        }
    }
}

Matrix::Matrix(int rows, int cols)
{
    init(rows, cols, MAT_TYPE_ZERO);
}

/*
 * Create new matrix with size rows*cols and having initial setup initMat.
 * initMat can be MATRIX_ZERO or MATRIX_IDENTITY.
 * Note that only square matrix could be identity matrix. Set initMat as MATRIX_IDENTITY for a
 * non-square matrix have no effect and the matrix is still set to zero.
 */
Matrix::Matrix(int rows, int cols, MatrixType initMat)
{
    init(rows, cols, initMat);
}

/*
 * Copy constructor
 */
Matrix::Matrix(const Matrix &mat)
{
    int i, j;

    rows = mat.rows;
    cols = mat.cols;

    cell = new double*[rows];
    for (i = 0; i < rows; i++) {
        cell[i] = new double[cols];
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            cell[i][j] = mat.cell[i][j];
        }
    }
}

Matrix::~Matrix()
{
    int i = 0;
    for (i = 0; i < rows; i++) {
        delete[] cell[i];
    }
    delete[] cell;
}

/*
 * Assign operator
 */
Matrix& Matrix::operator=(Matrix mat)
{
    // NOTE: copy constructor and a swap function are required
    swap(*this, mat);
    return *this;
}

/*
 * Multiply with a number. Syntax: Mat * a
 */
Matrix Matrix::operator*(double a)
{
    Matrix X = *this;
    for(int r = 0; r < X.rows; r++)
        for(int c = 0; c < X.cols; c++)
        {
            X.cell[r][c] *= a;
        };
    return X;
}

/* Add two matrices */
Matrix Matrix::operator+(const Matrix &mat)
{
    if(this->rows != mat.rows || this->cols != mat.cols) throw INVALID_MATRIX_SIZE;

    Matrix X(this->rows, this->cols);
    for (int r = 0; r < this->rows; r++)
        for (int c = 0; c < this->cols; c++)
            X.cell[r][c] = this->cell[r][c] + mat.cell[r][c];
    return X;
}

/* Minus matrix */
Matrix Matrix::operator-(const Matrix &mat)
{
    if(this->rows != mat.rows || this->cols != mat.cols) throw INVALID_MATRIX_SIZE;

    Matrix X(this->rows, this->cols);
    for (int r = 0; r < this->rows; r++)
        for (int c = 0; c < this->cols; c++)
            X.cell[r][c] = this->cell[r][c] - mat.cell[r][c];
    return X;
}

/* Multiply two matrices */
Matrix Matrix::operator*(const Matrix &mat)
{
    if(this->cols != mat.rows) throw INVALID_MATRIX_SIZE;

    Matrix X(this->rows, mat.cols);

    double tmp;
    for (int r = 0; r < X.rows; r++)
        for (int c = 0; c < X.cols; c++) {
            tmp = 0;
            for (int k = 0; k < this->cols; k++)
                tmp += this->cell[r][k] * mat.cell[k][c];
            X.cell[r][c] = tmp;
        };

    return X;
}

/* Get number of rows */
int Matrix::getRows()
{
    return rows;
}

/* Get number of columns */
int Matrix::getCols()
{
    return cols;
}

/* Get data in a cell */
double Matrix::getCell(int row, int col)
{
    return cell[row][col];
}

/* Set data in a cell */
void Matrix::setCell(int row, int col, double value)
{
    if (row >= 0 && col >= 0 && row < rows && col < cols) {
        cell[row][col] = value;
    }
}

/* Get a transposed matrix */
Matrix Matrix::transpose()
{
    Matrix X(this->cols, this->rows);
    for (int r = 0; r < X.rows; r++)
        for (int c = 0; c < X.cols; c++)
            X.cell[r][c] = this->cell[c][r];
    return X;
}

/*
 * Get sub-matrix corresponding to a cell. A sub-matrix corresponding to a cell [r][c] is a matrix
 * which equals to original matrix discarded row r and column c.
 */
Matrix Matrix::subMatrix(int r, int c)
{
    if (r < 0 || c < 0 || r >= rows || c >= cols) throw INVALID_SUBMATRIX_INDEX;

    Matrix X(rows - 1, cols - 1);
    int i, j, ii, jj;

    ii = 0;
    for (i = 0; i < rows; i++) {
        jj = 0;
        if (i != r) {
            for (j = 0; j < cols; j++) {
                if (j != c) {
                    X.setCell(ii, jj, cell[i][j]);
                    jj++;
                }
            }
            ii++;
        }
    }

    return X;
}

/*
 * Get determinant. Use only for square matrix. Call this function from a non-square matrix will
 * throw an exception.
 */
double Matrix::getDet()
{
    // Check if this is a square matrix
    if (rows != cols) throw INVALID_MATRIX_TYPE;

    if (rows == 1) return cell[0][0];

    int j;
    double det = 0;
    int prevSign = -1;
    for (j = 0; j < cols; j++) {
        prevSign *= -1;
        det += ((double) prevSign) * cell[0][j] * subMatrix(0, j).getDet();
    }

    return det;
}

/* Get inverted matrix. Call this function from an non-invertible matrix will throw an exception. */
Matrix Matrix::inverse()
{
    double detA = getDet(); // Determinant of this matrix

    if (detA == 0) throw INVALID_MATRIX_TYPE; // If detA == 0, the matrix is not invertible

    Matrix Ct(rows, cols); // Actually, this will be C^t * (1/detA)
    int i, j;
    int startSign = -1;
    int sign;

    for (i = 0; i < rows; i++) {
        sign = startSign;
        for (j = 0; j < cols; j++) {
            sign *= -1;
            Ct.cell[j][i] = ((double) sign) * subMatrix(i, j).getDet() / detA; // Calculate transposed C
        }
        startSign *= -1;
    }

    return Ct;
}

/* Print matrix to standard output */
void Matrix::print()
{
    int r, c;
    if (cell != NULL) {
        for (r = 0; r < rows; r++) {
            for (c = 0; c < cols; c++) {
                std::cout << cell[r][c] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}

void swap(Matrix &m1, Matrix &m2) {
    using std::swap;
    swap(m1.rows, m2.rows);
    swap(m1.cols, m2.cols);
    swap(m1.cell, m2.cell);
}

}  // namespace twsn
