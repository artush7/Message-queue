#include <pthread.h>
#include <queue>
#include <string>
#include <iostream>
#include <errno.h>
#include <stdexcept>
#include <unistd.h>
#include <vector>


class message_queue
{
private:
    static std::queue<std::string> queue;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
    size_t queue_size;
    pthread_t* producers;
    pthread_t* consumers;
    int producers_size;
    int consumers_size;

    
    static void* producer_thread(void* arg);
    static void* consumer_thread(void* arg);
    


public:

    message_queue();
    ~message_queue();
    void push(const std::string& msg);
    std::string pop();
    void start_message_queue_threads(int producers_size,int consumers_size);
    void join_thread();
    size_t size_queueu();
    
};


