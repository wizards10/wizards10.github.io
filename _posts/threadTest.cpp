#include "pthreadPool.hpp"
#include <iostream>
extern pthread_mutex_t m_cout;
class MyTask: public Task
{
public:
    MyTask()
    {}
    virtual int run()
    {
        pthread_mutex_lock(&m_cout);
        std::cout<<"threadID "<<pthread_self()<<" name is "<<(char*)this->arg_<<std::endl;
        pthread_mutex_unlock(&m_cout);
        sleep(1);
        return 0;
    }
};
int main()
{
    char tmp[] = "hello world";
    MyTask taskobj;
    taskobj.setArg((void*) tmp); 
    ThreadPool threadPool(10);
    for(int i = 0 ; i < 20 ; i++)
    {
        threadPool.addTask(&taskobj);
    } 
    while(1)  
    {   
        if (threadPool.size() == 0)
        {  
            threadPool.stop();
            printf("Now I will exit from main\n"); 
            exit(0);   
        }  
        sleep(2);  
    }  
    return 0;
}