/***********************************
* File:     openblastest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/5
***********************************/

#ifndef PERFORMANCE_OPTIMAZING_OPENBLASTEST_H
#define PERFORMANCE_OPTIMAZING_OPENBLASTEST_H

#include <Accelerate/Accelerate.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

namespace OpenblasTest {
    struct Matrix {
        int row;
        int col;
        float **data;
        Matrix(int row, int col) {
            assert(row > 0 && col > 0);
            this->row = row;
            this->col = col;
            data = new float* [row];
            for (int i = 0; i < row; ++i) {
                data[i] = new float[col];
            }

            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    data[i][j] = 0;
                }
            }
        }

        void randomize() {
            srand(time(nullptr));
            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    data[i][j] = rand() % 10;
                }
            }
        }

        void dumpMat() {
            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    printf("%f ", data[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }

        ~Matrix() {
            if (data) {
                for (int i = 0; i < row; ++i)
                    delete[] data[i];
                delete[] data;
            }
        }

        float get(int row, int col) const{
            return data[row][col];
        }
    };

    struct Vec {
        int row;
        float *data;

        Vec(const Vec& v) {
            std::cout << "copy construct..." << std::endl;
            row = v.row;
            for (int i = 0; i < v.row; ++i)
                data[i] = v.data[i];
        }

        Vec(Vec &&v) : data(v.data), row(v.row) {
            std::cout << "move construct..." << std::endl;
            v.data = NULL;
        }

        Vec& operator=(const Vec& v) {
            std::cout << "operator=" << std::endl;
            row = v.row;
            for (int i = 0; i < row; ++i)
                data[i] = v.data[i];
            return *this;
        }

        Vec(int row) {
            assert(row > 0);
            this->row = row;
            data = new float[row];
            for (int i = 0; i < row; ++i)
                data[i] = 0;
        }

        void randomize() {
            srand(time(nullptr));
            for (int i = 0; i < row; ++i)
                data[i] = rand() % 10;
        }

        void dumpVec() {
            printf("\n");
            for (int i = 0; i < row; ++i)
                printf("%f ", data[i]);
            printf("\n");
        }

        float& operator[](int i) {
            return data[i];
        }

        ~Vec() {
            if (data) {
                delete[] data;
            }
        }
    };
    class MyTest{
    public:
        Vec MatMulVec(Matrix &matrix, Vec &vec) {
            assert(matrix.col == vec.row);

            Vec v(matrix.row);

            for (int i = 0; i < matrix.row; ++i) {
                for (int j = 0; j < matrix.col; ++j) {
                    v[i] += matrix.get(i,j) * vec[j];
                }
            }

            return std::move(v);
        }
    };
}

#endif //PERFORMANCE_OPTIMAZING_OPENBLASTEST_H
