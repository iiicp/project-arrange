/**********************************
* File:     ThreadTest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/4
***********************************/

#ifndef MULTI_THREAD_THREADTEST_H
#define MULTI_THREAD_THREADTEST_H

#include <thread>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <mutex>
#include <shared_mutex>

namespace StdThreadTest {
    /// 类外的全局变量，才能作为 thread_local
    /// 每个线程单独拥有一份拷贝
    static thread_local FILE *fp = nullptr;
    class MyTest {
    public:
        MyTest() {
            looping = true;
            /**
             * 1. 此处是将对象成员函数作为线程执行函数，注意对象成员函数的第一个参数就是对象地址本身
             * 2. 此处展示了一个典型的将std::thread作为对象成员的使用
             */
             for (int i = 0; i < 5; ++i)
                 threadVec.push_back(std::thread(&MyTest::threadLoop, this));
        }

        ~MyTest() {
            looping = false;
            for (auto &t : threadVec)
                if (t.joinable())
                    t.join();
        }

    private:
        std::vector<std::thread> threadVec;

        bool looping;
        /**
         * 1. 隐含的第一个参数就是对象地址
         * 2. 将成员函数作为线程调用函数的好处是可以访问对象的所有属性
         */
        void threadLoop() {

            std::ostringstream ostr;
            ostr << std::this_thread::get_id();
            std::string fileName = "log_" + ostr.str() + ".txt";

            if (fp == nullptr) {
                fp = fopen(fileName.c_str(), "wb");
            }

            while (looping) {

                if (fp) {
                    fprintf(fp, "我是线程: %s\n", ostr.str().c_str());
                }
                usleep(500000);
            }
            if (fp) {
                fprintf(fp, "我是用std::thread创建的，我要退出了\n");
                fclose(fp);
            }
        }
    };
}

#endif//MULTI_THREAD_THREADTEST_H
