#include <pthread.h>
#include <queue>
#include <string>
#include <iostream>
#include <errno.h>
#include <stdexcept>


class message_queue
{
private:
    std::queue<std::string> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int queue_size = 10;
    pthread_t threads[10];
public:
    message_queue();
    ~message_queue();
    void push(const std::string& msg);
    
};


// void* receive(void* arg)
// {
//     while (true)
//     {
//         std::string local_msg;
//         std::cout << "Write a message" << std::endl;
//         std::getline(std::cin,local_msg);
//         
//         
//     }
//     return nullptr;
// }

// void* process(void* arg)
// {
//     while (true)
//     {
//         pthread_cond_wait(&cond,&mutex);
//         std::string local_msg = queue.front();
//         std::cout << "Process done" << std::endl;
//         queue.pop();
//         pthread_mutex_unlock(&mutex);
//     }
//     return nullptr;
// }
