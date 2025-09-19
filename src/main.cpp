#include "main.hpp"

bool message_queue::producer_done = false;

message_queue::message_queue()
{

    int pthr_mutex_ret = pthread_mutex_init(&mutex,nullptr);
    if ( pthr_mutex_ret != 0)
    {
        errno = pthr_mutex_ret;
        perror(" mutex initilization failed");
        throw std::runtime_error("");
    }

    int pthr_cond_full_ret = pthread_cond_init(&full,nullptr);
    if ( pthr_cond_full_ret != 0) 
    {
        errno = pthr_cond_full_ret;
        perror(" condition initilization failed");
        throw std::runtime_error("");
    }

    int pthr_cond_empty_ret = pthread_cond_init(&empty,nullptr);
    if ( pthr_cond_empty_ret != 0) 
    {
        errno = pthr_cond_empty_ret;
        perror(" condition initilization failed");
        throw std::runtime_error("");
    }
}

message_queue::~message_queue()
{
    join_thread();
    
    int pthr_mutex_dest_ret = pthread_mutex_destroy(&mutex);
    if (pthr_mutex_dest_ret != 0)
    {
        errno = pthr_mutex_dest_ret;
        perror("mutex destroying failed");
    }

    int pthr_cond_dest_full_ret = pthread_cond_destroy(&full);
    if ( pthr_cond_dest_full_ret != 0)
    {
        errno = pthr_cond_dest_full_ret;
        perror("condition destroying failed");
    }

    int pthr_cond_dest_empty_ret = pthread_cond_destroy(&empty);
    if ( pthr_cond_dest_empty_ret != 0)
    {
        errno = pthr_cond_dest_empty_ret;
        perror("condition destroying failed");
    }
    
    delete[] consumers;
    delete[] producers;
}

void message_queue::push(const std::string& msg)
{
    pthread_mutex_lock(&mutex);
    while (queue.size() >= queue_size)
    {
        pthread_cond_wait(&full,&mutex);
    }
    queue.push(msg);
    pthread_cond_broadcast(&empty);
    pthread_mutex_unlock(&mutex);
}

std::string message_queue::pop()
{
    pthread_mutex_lock(&mutex);
    while (queue.empty())
    {
        pthread_cond_wait(&empty,&mutex);
    }

    std::string msg = queue.front();
    queue.pop();
    pthread_cond_broadcast(&full);
    pthread_mutex_unlock(&mutex);
    return msg;

}

void* message_queue::producer_thread(void* arg)
{
    message_queue* m_q = static_cast<message_queue*>(arg);
    while(!producer_done)
    {
        int message_ID = rand();
        std::string msg = "Message ID : " + std::to_string(message_ID) + " something";
        m_q->push(msg);
        std::cout << msg << std::endl;
        sleep(1);
    }
    return nullptr;
}

void* message_queue::consumer_thread(void* arg)
{
    message_queue* m_q = static_cast<message_queue*>(arg);
    while (true)
    {
        if (producer_done)
        {
            break;
        }
        else
        {
            std::string msg = m_q->pop();
            std::cout << "Consumer got message : " << msg << std::endl;
        }


        sleep(1);
    }


    return nullptr;

}

void message_queue::start_message_queue_threads(int producers_size_,int consumers_size_)
{   
    producers_size = producers_size_;
    consumers_size = consumers_size_;

    producers = new pthread_t[producers_size];
    for(int i = 0; i < producers_size; ++i)
    {
        pthread_create(&producers[i],nullptr,&producer_thread,this);
    }

    consumers = new pthread_t[consumers_size];
    for(int i = 0; i < consumers_size; ++i)
    {
        pthread_create(&consumers[i],nullptr,&consumer_thread,this);
    }
}

void message_queue::join_thread()
{
    for(int i = 0; i < consumers_size;++i)
    {
        pthread_join(consumers[i],nullptr);
    }

    pthread_mutex_lock(&mutex);
    producer_done = true;
    pthread_cond_broadcast(&empty);
    pthread_mutex_unlock(&mutex);

    for(int i = 0; i < producers_size;++i)
    {
        pthread_join(producers[i],nullptr);
    }
}