#ifndef PTHREAD_POOL_
#define PTHREAD_POOL_
#include <deque>
#include <string>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

class Task
{
public:
    Task(void* arg = nullptr , const std::string taskname = "")
        :arg_(arg) , taskName_(taskname)
    {}
    ~Task()
    {

    }
    void setArg(void* arg)
    {
        arg_ = arg;
    }
    virtual int run() = 0 ;

private:
protected:
    void* arg_;
    std::string taskName_;
};

class ThreadPool
{
public:
    ThreadPool(int threadNum = 4);
    ~ThreadPool();
public:
    unsigned int addTask(Task* task);
    void stop();
    unsigned int size();
    Task* take();
private:
    int createThreads();
    static void* threadFunc(void* threadData);
private:
    ThreadPool& operator=(const ThreadPool&);
    ThreadPool(const ThreadPool&);
private:
    volatile bool isRunning_;
    int      threadNum_;
    pthread_t* threads;
    std::deque<Task*> taskQueue_;
    pthread_mutex_t mutex_;
    pthread_cond_t  condition_;
};

#endif