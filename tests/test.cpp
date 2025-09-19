#include <gtest/gtest.h>
#include "main.hpp"

TEST(msg_qu,test1)
{
    message_queue mq;
    mq.start_message_queue_threads(2,1);

}
