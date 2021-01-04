/**********************************
* File:     PthreadTest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/4
***********************************/

#ifndef MULTI_THREAD_PTHREADTEST_H
#define MULTI_THREAD_PTHREADTEST_H

#include <pthread.h>
#include <iostream>
#include <unistd.h>

/**
 * thread:  输出参数，会返回一个线程id
 * attr:    创建线程时设置的属性，传NULL表示设置为默认属性
 * start_routine: 函数指针，传入一个线程执行函数，这个函数必须是__cdecl，即c declaration
 * arg:     给线程函数传入一个参数
 *
 * 返回值
 * 创建成功返回   0
 * 创建失败 : 创建返回错误吗 EAGAIN（系统资源不足时）, EINVAL (传入了非法的attri)
 * 实际过程中，只要保证正确的初始化，我们一般不需要关心返回值。
 *
 * int pthread_create(pthread_t *thread,
 *                    const pthread_attr_t *attr,
 *                    void *(*start_routine)(void *),
 *                    void *arg);
 */

/**
 * 此处妙用了c++ lambda函数，兼容了pthread的调用签名，实际调用的是c++成员函数. 妙妙!!!
 *
 * 参考：https://zhou-yuxin.github.io/articles/2017/C++11%20%E6%88%90%E5%91%98%E5%87%BD%E6%95%B0%E4%BD%9C%E4%B8%BApthread%E7%BA%BF%E7%A8%8B/index.html
 */

namespace PthreadTest {
    class MyTest {
    public:
        MyTest() {
            isLooping = true;
            pthread_create(&thread, NULL,
                    [](void *args) {
                        MyTest *test = (MyTest *)args;
                        test->threadLoop();
                        return (void *)0; /// 对于pthread，这种方式只能是返回 (void *)0
                    }, this);
        }

        ~MyTest() {
            isLooping = false;
            int *retVal = nullptr;
            pthread_join(thread, (void **)&retVal);
            std::cout << "is ok ... " << std::endl;
        }
    private:
        bool isLooping;
        pthread_t thread;
        void *threadLoop() {
            while (isLooping) {
                std::cout << "我是使用pthread创建的，我的id是: " << pthread_self() << std::endl;
                usleep(500000);
            }
            std::cout << "我是使用pthread创建的，我要退出了" << std::endl;

            return NULL;
        }
    };
}

#endif//MULTI_THREAD_PTHREADTEST_H
