#include <iostream>
#include "openblastest.h"
#include <time.h>

int main() {

    OpenblasTest::MyTest mytest;
    OpenblasTest::Matrix mat(256,161);
    mat.randomize();
    //mat.dumpMat();

    OpenblasTest::Vec vec(161);
    vec.randomize();
    //vec.dumpVec();

    clock_t start = clock();
    OpenblasTest::Vec v = mytest.MatMulVec(mat, vec);
    clock_t end = clock();
    std::cout << "time: " << (double)(end - start) / 1000 << "ms" << std::endl;
    //v.dumpVec();

    return 0;
}
