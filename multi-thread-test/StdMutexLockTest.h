/**********************************
* File:     StdLockTest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/4
***********************************/

#ifndef MULTI_THREAD_STDLOCKTEST_H
#define MULTI_THREAD_STDLOCKTEST_H

#include <thread>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <unistd.h>

namespace StdMutexLockTest {
    class MyTest {
    public:
        MyTest() {
            a = 0;
            isLooping = true;
            incThread = std::thread(&MyTest::incThreadLoop, this);
            decThread = std::thread(&MyTest::decThreadLoop, this);
        }

        ~MyTest() {
            isLooping = false;
            if (incThread.joinable())
                incThread.join();
            if (decThread.joinable())
                decThread.join();
        }

        int getA() {
            return a;
        }
    private:
        int a;
        std::mutex mutex;
        std::thread incThread;
        std::thread decThread;
        bool isLooping;
        void incThreadLoop() {
            while (isLooping) {
                usleep(5000000);
                std::lock_guard<std::mutex> lg(mutex);
                for (int i = 0; i < 1001; ++i) {
                    a++;
                }
                break;
            }
        }

        void decThreadLoop() {
            while (isLooping) {
                usleep(5000000);
                std::lock_guard<std::mutex> lg(mutex);
                for (int i = 0; i < 1000; ++i) {
                    a--;
                }
                break;
            }
        }
    };
}

#endif//MULTI_THREAD_STDLOCKTEST_H
