#include <iostream>
#include "openblastest.h"
#include <time.h>

int main() {

    OpenblasTest::MyTest mytest;
    
    // Mat x Mat  C = alpha * A * B + beta * C
    int row1 = 256, col1 = 128;
    int row2 = 128, col2 = 64;
    OpenblasTest::Matrix<float> mat(row1,col1);
    OpenblasTest::Matrix<float> mat2(row2,col2);
    mat.randomize();
    mat2.randomize();

    std::cout << "C = alpha * A * B + beta * C: " << "[" << row1 << "," << col1 <<"] x " << "[" << row2 << "," << col2 << "]" << std::endl;
    clock_t start = clock();
    OpenblasTest::Matrix<float> m = mytest.MatMulMat(mat, mat2);
    clock_t end = clock();
    std::cout << "native cpp time: " << (double)(end - start) / 1000 << "ms" << std::endl;

    start = clock();
    OpenblasTest::Matrix<float> m_blas = mytest.MatMulMat_blas(mat, mat2);
    end = clock();
    std::cout << "openblas time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    
    // Mat x Vec  C = alpha * A * b + beta * C;
    row1 = 256;
    col1 = 161;
    OpenblasTest::Matrix<float> mat3(row1,col1);
    mat3.randomize();
    //mat3.dumpMat();
    
    row2 = 161;
    OpenblasTest::Vec<float> vec(row2);
    vec.randomize();
    //vec.dumpVec();
    
    std::cout << "C = alpha * A * b + beta * C: " << "[" << row1 << "," << col1 <<"] x " << "[" << row2 << ", 1"<< "]" << std::endl;
    start = clock();
    OpenblasTest::Vec<float> c = mytest.MatMulVec(mat3, vec);
    end = clock();
    //c.dumpVec();
    std::cout << "native cpp time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    
    start = clock();
    OpenblasTest::Vec<float> c_blas = mytest.MatMulVec_blas(mat3, vec);
    end = clock();
    //c_blas.dumpVec();
    std::cout << "openblas time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    
    // Vec + Vec  y = alpha * x + y;
    row1 = 512;
    OpenblasTest::Vec<float> vec3(row1);
    vec3.randomize();
    OpenblasTest::Vec<float> vec4(row1);
    vec4.randomize();
    OpenblasTest::Vec<float> vec5(vec4);
    
    std::cout << "y = alpha * x + y: " << "[" << row1 << ",1" <<"] + " << "[" << row1 << ",1" <<"]" << std::endl;
    start = clock();
    mytest.VecAddVec(vec3, vec4, 1.0f);
    end = clock();
    std::cout << "native cpp time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    
    start = clock();
    mytest.VecAddVec_blas(vec3, vec5, 1.0f);
    end = clock();
    std::cout << "openblas time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    return 0;
}
