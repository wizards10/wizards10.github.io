#include <iostream>
#include <stdlib.h>
#include "pthreadPool.hpp"
pthread_mutex_t m_cout;
ThreadPool::ThreadPool(int threadNum)
{
    isRunning_ = true;
    threadNum_ = threadNum;
    createThreads();
}

ThreadPool::~ThreadPool()
{
    stop();
    for(auto itr : taskQueue_)
    {
        delete itr;
    }
    taskQueue_.clear();
}

int ThreadPool::createThreads()
{
    pthread_mutex_init(&m_cout, nullptr);
    pthread_mutex_init(&mutex_ , nullptr);
    pthread_cond_init(&condition_,nullptr);
    threads = new pthread_t[threadNum_];
    for(int i = 0 ; i < threadNum_ ;i++)
    {
        pthread_create(&threads[i] , nullptr , threadFunc , (void*)this);
    }
    return 0;
}

unsigned int ThreadPool::addTask(Task* task)
{
    pthread_mutex_lock(&mutex_);
    taskQueue_.push_back(task);
    unsigned int size = taskQueue_.size();
    pthread_mutex_unlock(&mutex_);
    pthread_cond_signal(&condition_);
    return size;
}

void ThreadPool::stop()
{
    if(!isRunning_)
    {
        return;
    }
    isRunning_ = false;
    pthread_cond_broadcast(&condition_);
    for(int i = 0 ; i < threadNum_ ; i++)
    {
        pthread_join(threads[i] , nullptr);
    }
    delete[] threads;
    threads = nullptr;
    pthread_mutex_destroy(&mutex_);
    pthread_cond_signal(&condition_);
    return;
}

unsigned int ThreadPool::size()
{
    pthread_mutex_lock(&mutex_);
    unsigned int size = taskQueue_.size();
    pthread_mutex_unlock(&mutex_);
    return size;
}

Task* ThreadPool::take()
{
    Task* task = nullptr;
    while(!task)
    {
        pthread_mutex_lock(&mutex_);
        while(taskQueue_.empty() && isRunning_)
        {
            pthread_cond_wait(&condition_ , &mutex_);
        }
        
        if(!isRunning_)
        {
            pthread_mutex_unlock(&mutex_);
            break;
        }

        else if(taskQueue_.empty())
        {
            pthread_mutex_unlock(&mutex_);
            continue;
        }
        task = taskQueue_.front();
        taskQueue_.pop_front();
        printf("there are still %d tasks need to process\n", taskQueue_.size()); 
        pthread_mutex_unlock(&mutex_);

    }
    return task;
}
void* ThreadPool::threadFunc(void* threadData)
{
    pthread_t tid = pthread_self();
    ThreadPool* pool = static_cast<ThreadPool*>(threadData);
    while(pool->isRunning_)
    {
        Task* task = pool->take();
        if(task == nullptr)
        {
            std::cout<<"thread "<<tid<<"will exit"<<std::endl;
            break;
        }
        task->run();
    }
    return 0;
}

