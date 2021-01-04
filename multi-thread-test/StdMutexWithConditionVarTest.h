/**********************************
* File:     StdMutexWithConditionVarTest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/4
***********************************/

#ifndef MULTI_THREAD_STDMUTEXWITHCONDITIONVARTEST_H
#define MULTI_THREAD_STDMUTEXWITHCONDITIONVARTEST_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <shared_mutex>
#include <unistd.h>

namespace StdMutexWithConditionVarTest {
    class MyTest {
    public:
        MyTest() {
            isLooping = true;
            isNotify = false;
            thread = std::thread(&MyTest::threadLoop, this);
        }
        ~MyTest() {
            isLooping = false;
            notify();
            if (thread.joinable())
                thread.join();
        }

        void notify() {
            std::unique_lock<std::mutex> ul(mutex);
            isNotify = true;
            conditionVar.notify_one();
        }

    private:
        bool isLooping;
        bool isNotify;
        std::mutex mutex;
        std::condition_variable conditionVar;
        std::thread thread;

        void threadLoop() {
            while (isLooping) {
                std::unique_lock<std::mutex> ul(mutex);
                std::cout << "测试条件变量 1" << std::endl;
                while (isNotify == false)
                    conditionVar.wait(ul);
                isNotify = false;
                std::cout << "测试条件变量 2" << std::endl;
            }
            std::cout << "测试条件变量线程退出" << std::endl;
        }
    };
}

#endif//MULTI_THREAD_STDMUTEXWITHCONDITIONVARTEST_H
