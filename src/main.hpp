#include <pthread.h>
#include <queue>
#include <string>
#include <iostream>
#include <errno.h>
#include <stdexcept>
#include <unistd.h>


class message_queue
{
private:
    std::queue<std::string> queue;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
    const int queue_size = 10;
    pthread_t* producers;
    pthread_t* consumers;
    int producers_size;
    int consumers_size;
    static bool producer_done;

    
    static void* producer_thread(void* arg);
    static void* consumer_thread(void* arg);


public:

    message_queue();
    ~message_queue();
    void push(const std::string& msg);
    std::string pop();
    void start_message_queue_threads(int producers_size,int consumers_size);
    void join_thread();
    
};


