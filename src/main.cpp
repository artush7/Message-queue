#include <pthread.h>
#include <queue>
#include <string>
#include <iostream>

std::queue<std::string> queue;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int queue_size = 10;
pthread_t threads[10];


void* receive(void* arg)
{
    while (true)
    {
        std::string local_msg;
        std::cout << "Write a message" << std::endl;
        std::getline(std::cin,local_msg);
        pthread_mutex_lock(&mutex);
        queue.push(local_msg);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* process(void* arg)
{
    while (true)
    {
        pthread_cond_wait(&cond,&mutex);
        std::string local_msg = queue.front();
        std::cout << "Process done" << std::endl;
        queue.pop();
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}
