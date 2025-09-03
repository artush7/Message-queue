#include "main.hpp"

message_queue::message_queue()
{
    int pthr_mutex_ret = pthread_mutex_init(&mutex,nullptr);
    if ( pthr_mutex_ret != 0)
    {
        errno = pthr_mutex_ret;
        perror(" mutex initilization failed");
        throw std::runtime_error("");
    }

    int pthr_cond_ret = pthread_cond_init(&cond,nullptr);
    if ( pthr_cond_ret != 0) 
    {
        errno = pthr_cond_ret;
        perror(" condition initilization failed");
        throw std::runtime_error("");
    }
}

message_queue::~message_queue()
{
    int pthr_mutex_dest_ret = pthread_mutex_destroy(&mutex);
    if (pthr_mutex_dest_ret != 0)
    {
        errno = pthr_mutex_dest_ret;
        perror("mutex destroying failed");
    }

    int pthr_cond_dest_ret = pthread_cond_destroy(&cond);
    if ( pthr_cond_dest_ret != 0)
    {
        errno = pthr_cond_dest_ret;
        perror("condition destroying failed");
    }
    
}

void message_queue::push(const std::string& msg)
{
    pthread_mutex_lock(&mutex);
    queue.push(msg);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}