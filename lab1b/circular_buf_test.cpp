#include "circular_buf.h"
#include <gtest/gtest.h>


TEST(CircularBufferTest, ConstructorDefault) {
    CircularBuffer cbuf;
    EXPECT_TRUE(cbuf.empty());
    EXPECT_EQ(cbuf.size(), 0);
}

TEST(CircularBufferTest, ConstructorWithCapacity) {
    CircularBuffer cbuf(10);
    EXPECT_EQ(cbuf.capacity(), 10);
    EXPECT_EQ(cbuf.size(), 0);
    EXPECT_EQ(cbuf[0], false);
    EXPECT_EQ(cbuf[9], false);
}

TEST(CircularBufferTest, ConstructorWithCapacityAndValue) {
    CircularBuffer cbuf(10, 5);
    EXPECT_EQ(cbuf.capacity(), 10);
    EXPECT_EQ(cbuf.size(), 10);
    EXPECT_EQ(cbuf[0], 5);
    EXPECT_EQ(cbuf[9], 5);
}

TEST(CircularBufferTest, CopyConstructor) {
    CircularBuffer cbuf1(10, 5);
    CircularBuffer cbuf2(cbuf1);
    EXPECT_EQ(cbuf1.capacity(), cbuf2.capacity());
    EXPECT_EQ(cbuf1.size(), cbuf2.size());
    EXPECT_EQ(cbuf1[0], cbuf2[0]);
    EXPECT_EQ(cbuf1[9], cbuf2[9]);
}

TEST(CircularBufferTest, PushBack) {
    CircularBuffer cbuf(10);
    cbuf.push_back(5);
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 5);
    cbuf.push_back(6);
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 5);
    EXPECT_EQ(cbuf[1], 6);
    cbuf.push_back(7);
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 5);
    EXPECT_EQ(cbuf[1], 6);
    EXPECT_EQ(cbuf[2], 7);
    cbuf.push_back(8);
    EXPECT_EQ(cbuf.size(), 4);
    EXPECT_EQ(cbuf[0], 5);
}

TEST(CircularBufferTest, PushFront) {
    CircularBuffer cbuf(10);
    cbuf.push_front(5);
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 5);
    cbuf.push_front(6);
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 6);
    EXPECT_EQ(cbuf[1], 5);
    cbuf.push_front(7);
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 7);
    EXPECT_EQ(cbuf[1], 6);
    EXPECT_EQ(cbuf[2], 5);
    cbuf.push_front(8);
    EXPECT_EQ(cbuf.size(), 4);
    EXPECT_EQ(cbuf[0], 8);
    EXPECT_EQ(cbuf[1], 7);
    EXPECT_EQ(cbuf[2], 6);
    EXPECT_EQ(cbuf[3], 5);
}

TEST(CircularBufferTest, PopBack) {
    CircularBuffer cbuf(10, 5);
    cbuf.pop_back();
    EXPECT_EQ(cbuf.size(), 9);
    EXPECT_EQ(cbuf[0], 5);
    cbuf.pop_back();
    EXPECT_EQ(cbuf.size(), 8);
    EXPECT_EQ(cbuf[0], 5);
}

TEST(CircularBufferTest, PopFront) {
    CircularBuffer cbuf(10, 5);
    cbuf.pop_front();
    EXPECT_EQ(cbuf.size(), 9);
    EXPECT_EQ(cbuf[0], 5);
    cbuf.pop_front();
    EXPECT_EQ(cbuf.size(), 8);
    EXPECT_EQ(cbuf[0], 5);
}

TEST(CircularBufferTest, FrontElement) {
    CircularBuffer cbuf(10, 5);
    EXPECT_EQ(cbuf.front(), 5);
    CircularBuffer cbuf1(5);  //////////////////////////??????????????
    EXPECT_EQ(cbuf1.front(), false);
}
