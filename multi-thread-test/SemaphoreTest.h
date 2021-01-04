/**********************************
* File:     SemaphoreTest.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2021/1/4
***********************************/

#ifndef MULTI_THREAD_SEMAPHORETEST_H
#define MULTI_THREAD_SEMAPHORETEST_H

#include <thread>
#include <iostream>
#include <semaphore.h>
#include <vector>
#include <mutex>

namespace SemaphoreTest {
    class MyTest {
    public:
        MyTest() {
            isLooping = true;
            taskId = 0;
            sem_init(&sem, 0, 0);
            producerThread = std::thread(&MyTest::producerThreadLoop, this);
            for (int i = 0; i < 5; ++i)
                threadVec.push_back(std::thread(&MyTest::consumeThreadLoop, this));
        }
        ~MyTest() {
            isLooping = false;
            sem_post(&sem);
            sem_destroy(&sem);
            if (producerThread.joinable())
                producerThread.join();

            for (auto &th : threadVec)
                if (th.joinable())
                    th.join();
        }
    private:
        std::thread producerThread;
        std::vector<std::thread> threadVec;
        std::mutex mutex;
        sem_t sem;
        bool isLooping;
        int taskId;

        void producerThreadLoop() {
            while (isLooping) {
                mutex.lock();
                taskId++;
                std::cout << "produce a task, taskId: " << taskId << std::endl;
                mutex.unlock();
                sem_post(&sem);
                sleep(1);
            }
        }

        void consumeThreadLoop() {
            while (isLooping) {
                while (sem_wait(&sem) != 0) {
                    usleep(500000);
                }
                if (taskId > 0) {
                    mutex.lock();
                    if (taskId > 0) {
                        taskId--;
                        std::cout << "consume a task, taskId: " << taskId << ", threadId: " << std::this_thread::get_id() << std::endl;
                    }
                    mutex.unlock();
                }
                usleep(500000);
            }
        }
    };
}

#endif//MULTI_THREAD_SEMAPHORETEST_H
