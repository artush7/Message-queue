#include <gtest/gtest.h>
#include "main.hpp"

TEST(mq, push) 
{
    message_queue mq;
    mq.push("Hello");
    std::string result = mq.pop();
    EXPECT_EQ(result, "Hello");
}

TEST(mq, pop)
{
    message_queue mq;

    mq.push("first");
    mq.push("second");
    mq.push("third");

    EXPECT_EQ(mq.pop(), "first");
    EXPECT_EQ(mq.pop(), "second");
    EXPECT_EQ(mq.pop(), "third");
}

TEST(mq, working)
{
    message_queue mq;

    mq.start_message_queue_threads(2, 2);

    sleep(3);

    EXPECT_GE(mq.size_queueu(), 0);

}


TEST(mq, working_2) {
    message_queue mq;


    mq.start_message_queue_threads(150, 150);

    sleep(5);

    EXPECT_GE(mq.size_queueu(), 0);

    mq.push("FinalMessage");
    std::string msg = mq.pop();
    EXPECT_EQ(msg, "FinalMessage");
}


