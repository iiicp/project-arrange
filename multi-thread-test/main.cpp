#include <iostream>
#include <unistd.h>
#include "StdThreadTest.h"
#include "PthreadTest.h"
#include "StdMutexLockTest.h"
#include "StdMutexWithConditionVarTest.h"
#include "SemaphoreTest.h"

int main() {

    StdThreadTest::MyTest *test = new StdThreadTest::MyTest();
    PthreadTest::MyTest * test2 = new PthreadTest::MyTest();
    StdMutexLockTest::MyTest *test3 = new StdMutexLockTest::MyTest();
    StdMutexWithConditionVarTest::MyTest *test4 = new StdMutexWithConditionVarTest::MyTest();
    SemaphoreTest::MyTest *test5 = new SemaphoreTest::MyTest();
    std::cout << test3->getA() << std::endl;

    while (true) {
        sleep(5);
        break;
    }
    delete test;
    delete test2;

    std::cout << test3->getA() << std::endl;
    test4->notify();

    while (true) {
        sleep(5);
        break;
    }
    delete test4;
    while (true) {
        sleep(1000);
    }
    return 0;
}
