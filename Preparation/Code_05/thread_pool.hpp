//
// Created by larry on 23-4-17.
//

#ifndef TINY_WEB_SERVER_THREAD_POOL_H
#define TINY_WEB_SERVER_THREAD_POOL_H

#include <pthread.h>
#include <list>
#include <cstdio>
#include <exception>
#include "locker.hpp"

// 线程池类，定义成模板类是为了代码复用
template<typename T>
class ThreadPool {
public:
    ThreadPool(int thread_number = 8, int max_request = 10000);

    ~ThreadPool();

    bool append(T *request);

private:
    static void *worker(void *arg);

    void run();

private:
    int m_thread_number;     // 线程的数量
    pthread_t *m_threads; // 线程池数组，大小为 m_thread_number
    int m_max_requests; // 请求队列中最多运行的等待处理的请求数量
    std::list<T *> m_work_queue; // 请求队列
    Locker m_queue_locker; // 互斥锁
    Sem m_queue_stat; // 信号量用来判断是否有任务需要处理
    bool m_stop; // 是否结束线程
};

template<typename T>
ThreadPool<T>::ThreadPool(int thread_number, int max_request):
        m_thread_number(thread_number), m_threads(nullptr),
        m_max_requests(max_request), m_stop(false) {

    if ((thread_number <= 0) || (max_request <= 0)) {
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_number];
    if (!m_threads) {
        throw std::exception();
    }

    // 创建 thread_thread 个线程，并将它们设置为线程脱离
    for (int i = 0; i < thread_number; ++i) {
        printf("create the %dth thread\n", i);
        if (pthread_create(m_threads + i, NULL, worker, this) != 0) {
            delete[] m_threads;
            throw std::exception();
        }

        if (pthread_detach(m_threads[i])) {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
ThreadPool<T>::~ThreadPool() {
    delete[] m_threads;
    m_stop = true;
}

template<typename T>
bool ThreadPool<T>::append(T *request) {
    // 操作工作队列时一定要加锁，因为它被所有线程共享。
    m_queue_locker.lock();
    if (m_work_queue.size() > m_max_requests) {
        m_queue_locker.unlock();
        return false;
    }

    m_work_queue.push_back(request);
    m_queue_locker.unlock();
    m_queue_stat.post();
    return true;
}

template<typename T>
void *ThreadPool<T>::worker(void *arg) {
    auto *pool = (ThreadPool *) arg;
    pool->run();
    return pool;
}

template<typename T>
void ThreadPool<T>::run() {
    while (!m_stop) {
        m_queue_stat.wait();
        m_queue_locker.lock();
        if (m_work_queue.empty()) {
            m_queue_locker.unlock();
            continue;
        }

        T *request = m_work_queue.front();
        m_work_queue.pop_front();
        m_queue_locker.unlock();

        if (!request) {
            continue;
        }
        request->process();
    }
}

#endif //TINY_WEB_SERVER_THREAD_POOL_H
