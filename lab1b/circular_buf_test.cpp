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
    EXPECT_THROW(cbuf[0], std::invalid_argument);
    EXPECT_THROW(cbuf[9], std::invalid_argument);
}

TEST(CircularBufferTest, ConstructorWithCapacityAndValue) {
    CircularBuffer cbuf(10, 'A');
    EXPECT_EQ(cbuf.capacity(), 10);
    EXPECT_EQ(cbuf.size(), 10);
    EXPECT_EQ(cbuf[0], 'A');
    EXPECT_EQ(cbuf[9], 'A');
}

TEST(CircularBufferTest, CopyConstructor) {
    CircularBuffer cbuf1(10, 'A');
    CircularBuffer cbuf2(cbuf1);
    EXPECT_EQ(cbuf1.capacity(), cbuf2.capacity());
    EXPECT_EQ(cbuf1.size(), cbuf2.size());
    EXPECT_EQ(cbuf1[0], cbuf2[0]);
    EXPECT_EQ(cbuf1[9], cbuf2[9]);
}

TEST(CircularBufferTest, OperatorBrackets) {
    CircularBuffer cbuf(10, 'A');

    EXPECT_EQ(cbuf[0], 'A');
    EXPECT_EQ(cbuf[9], 'A');

    CircularBuffer cbuf2;
    EXPECT_THROW(cbuf2[0], std::invalid_argument);

    const CircularBuffer cbuf3(10, 'B');
    EXPECT_EQ(cbuf3[0], 'B');
}


TEST(CircularBufferTest, At) {
    CircularBuffer cbuf(10, 'A');
    EXPECT_EQ(cbuf.at(0), 'A');
    EXPECT_EQ(cbuf.at(9), 'A');
    EXPECT_THROW(cbuf.at(10), std::out_of_range);
    EXPECT_THROW(cbuf.at(-1), std::out_of_range);

    const CircularBuffer cbuf2(10, 'B');
    EXPECT_EQ(cbuf2.at(0), 'B');
    EXPECT_THROW(cbuf2.at(10), std::out_of_range);
    EXPECT_THROW(cbuf2.at(-1), std::out_of_range);
}

TEST(CircularBufferTest, Front) {
    CircularBuffer cbuf(5, 'A');
    EXPECT_EQ(cbuf.front(), cbuf[0]);
    EXPECT_EQ(cbuf.front(), 'A');

    const CircularBuffer cbuf2(5, 'B');
    EXPECT_EQ(cbuf2.front(), cbuf2[0]);
    EXPECT_EQ(cbuf2.front(), 'B');
}

TEST(CircularBufferTest, Back) {
    CircularBuffer cbuf(5, 'A');
    EXPECT_EQ(cbuf.back(), cbuf[4]);
    EXPECT_EQ(cbuf.back(), 'A');

    const CircularBuffer cbuf2(5, 'B');
    EXPECT_EQ(cbuf2.back(), cbuf2[4]);
    EXPECT_EQ(cbuf2.back(), 'B');
}

TEST(CircularBufferTest, Linearise) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');
    cbuf.push_back('C');
    cbuf.push_back('D');

    cbuf.linearize();
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'B');
    EXPECT_EQ(cbuf[1], 'C');
    EXPECT_EQ(cbuf[2], 'D');

    cbuf.pop_front();
    cbuf.linearize();
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 'C');
    EXPECT_EQ(cbuf[1], 'D');
}


TEST(CircularBufferTest, IsLinearized) {
    CircularBuffer cbuf(3);

    EXPECT_TRUE(cbuf.is_linearized());
    cbuf.push_back('A');
    EXPECT_TRUE(cbuf.is_linearized());
    cbuf.push_back('B');
    cbuf.push_back('C');
    cbuf.push_back('D');
    EXPECT_FALSE(cbuf.is_linearized());
}
TEST(CircularBufferTest, Rotate) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');
    cbuf.push_back('C');

    cbuf.rotate(2);
    EXPECT_EQ(cbuf[0], 'C');
    EXPECT_EQ(cbuf[1], 'A');
    EXPECT_EQ(cbuf[2], 'B');

    EXPECT_THROW(cbuf.rotate(3), std::out_of_range);
    EXPECT_THROW(cbuf.rotate(-1), std::out_of_range);

}

TEST(CircularBufferTest, Size) {
    CircularBuffer cbuf(3);

    EXPECT_EQ(cbuf.size(), 0);

    cbuf.push_back('A');
    EXPECT_EQ(cbuf.size(), 1);
}

TEST(CircularBufferTest, Empty) {
    CircularBuffer cbuf(3);

    EXPECT_TRUE(cbuf.empty());

    cbuf.push_back('A');
    EXPECT_FALSE(cbuf.empty());

    cbuf.pop_front();
    EXPECT_TRUE(cbuf.empty());
}

TEST(CircularBufferTest, Full) {
    CircularBuffer cbuf(3, 'A');
    EXPECT_TRUE(cbuf.full());

    cbuf.pop_back();
    EXPECT_FALSE(cbuf.full());

    CircularBuffer cbuf2(3);
    EXPECT_FALSE(cbuf2.full());
}

TEST(CircularBufferTest, Reserve) {
    CircularBuffer cbuf(3);
    EXPECT_EQ(cbuf.reserve(), 3);
    cbuf.push_back('A');
    EXPECT_EQ(cbuf.reserve(), 2);
}

TEST(CircularBufferTest, Capacity) {
    CircularBuffer cbuf(3);
    EXPECT_EQ(cbuf.capacity(), 3);
    }

TEST(CircularBufferTest, SetCapacity) {
    CircularBuffer cbuf(3);
    cbuf.set_capacity(5);
    EXPECT_EQ(cbuf.capacity(), 5);
    EXPECT_THROW(cbuf.set_capacity(-1);, std::invalid_argument);

    cbuf.push_back('A');
    cbuf.push_back('B');
    EXPECT_THROW(cbuf.set_capacity(1);, std::invalid_argument);
}

TEST(CircularBufferTest, Resize) {
    CircularBuffer cbuf(5);    // size = 0, capacity = 5
    cbuf.resize(3, 'A');
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'A');
    EXPECT_EQ(cbuf[2], 'A');

    cbuf.resize(2, 'B');
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 'A');
    EXPECT_EQ(cbuf[1], 'A');

    EXPECT_THROW(cbuf.resize(-1, 'A'), std::invalid_argument);
    EXPECT_THROW(cbuf.resize(10, 'A'), std::invalid_argument);
}

TEST(CircularBufferTest, AssigmentOperator) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');
    cbuf.push_back('C');
    CircularBuffer cbuf2(5);
    cbuf2 = cbuf;
    EXPECT_EQ(cbuf2.size(), cbuf.size());
    EXPECT_EQ(cbuf2[0], 'A');
    EXPECT_EQ(cbuf2[2], 'C');
    EXPECT_EQ(cbuf2.capacity(), cbuf.capacity());
}

TEST(CircularBufferTest, Swap) {
    CircularBuffer cbuf(2);
    cbuf.push_back('A');
    cbuf.push_back('B');
    CircularBuffer cbuf2(4);
    cbuf2.push_back('C');
    cbuf2.push_back('D');
    cbuf2.push_back('E');
    CircularBuffer cbuf3 = cbuf;
    CircularBuffer cbuf4 = cbuf2;
    cbuf.swap(cbuf2);
    EXPECT_EQ(cbuf.size(), cbuf4.size()); // 3
    EXPECT_EQ(cbuf2.size(), cbuf3.size()); // 2
    EXPECT_EQ(cbuf.capacity(), cbuf4.capacity()); // 4
    EXPECT_EQ(cbuf2.capacity(), cbuf2.capacity()); // 2
    EXPECT_EQ(cbuf[0], 'C');
    EXPECT_EQ(cbuf[1], 'D');
    EXPECT_EQ(cbuf[2], 'E');
    EXPECT_EQ(cbuf2[0], 'A');
    EXPECT_EQ(cbuf2[1], 'B');
}

TEST(CircularBufferTest, PushBack) {
    CircularBuffer cbuf(3);

    cbuf.push_back('A');
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 'A');

    cbuf.push_back('B');
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[1], 'B');

    cbuf.push_back('C');
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[2], 'C');

    cbuf.push_back('D');   // начинаем переписывать 0-й элемент
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'B');
    EXPECT_EQ(cbuf[1], 'C');
    EXPECT_EQ(cbuf[2], 'D');
}


TEST(CircularBufferTest, PushFront) {
    CircularBuffer cbuf(3);

    cbuf.push_front('A');
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 'A');

    cbuf.push_front('B');
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 'B');

    cbuf.push_front('C');
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'C');

    cbuf.push_front('D');   // начинаем переписывать 0-й элемент
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'D');
    EXPECT_EQ(cbuf[1], 'C');
    EXPECT_EQ(cbuf[2], 'B');
}

TEST(CircularBufferTest, PushBackFrontZeroCapacity) {
    CircularBuffer cbuf(0);
    EXPECT_THROW(cbuf.push_back(5), std::invalid_argument);
    EXPECT_THROW(cbuf.push_front(5), std::invalid_argument);
}

TEST(CircularBufferTest, PopBack) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');

    cbuf.pop_back();
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 'A');

    EXPECT_EQ(cbuf.front(), 'A');
    EXPECT_EQ(cbuf.back(), 'A');
}

TEST(CircularBufferTest, PopFront) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');

    cbuf.pop_front();
    EXPECT_EQ(cbuf.size(), 1);
    EXPECT_EQ(cbuf[0], 'B');

    EXPECT_EQ(cbuf.front(), 'B');
    EXPECT_EQ(cbuf.back(), 'B');
}

TEST(CircularBufferTest, Insert) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');

    cbuf.insert(1, 'C');
    EXPECT_EQ(cbuf.size(), 3);
    EXPECT_EQ(cbuf[0], 'A');
    EXPECT_EQ(cbuf[1], 'C');
    EXPECT_EQ(cbuf[2], 'B');

    EXPECT_THROW(cbuf.insert(-1, 'C'), std::out_of_range);
    EXPECT_THROW(cbuf.insert(4, 'C'), std::out_of_range);

    CircularBuffer cbuf2(1);
    cbuf2.insert(0, 'A');
    EXPECT_EQ(cbuf2[0], 'A');
    EXPECT_THROW(cbuf2.insert(0, 'B'), std::overflow_error);
}


TEST(CircularBufferTest, Erase) {
    CircularBuffer cbuf(3);
    cbuf.push_back('A');
    cbuf.push_back('B');
    cbuf.push_back('C');

    cbuf.erase(0, 1);
    EXPECT_EQ(cbuf.size(), 2);
    EXPECT_EQ(cbuf[0], 'B');
    EXPECT_EQ(cbuf[1], 'C');

    EXPECT_THROW(cbuf.erase(-1, 1), std::out_of_range);
    EXPECT_THROW(cbuf.erase(3, 1), std::out_of_range);
}

TEST(CircularBufferTest, Clear) {
    CircularBuffer cbuf(2);
    cbuf.push_back('A');
    cbuf.push_back('B');

    cbuf.clear();
    EXPECT_EQ(cbuf.size(), 0);
    EXPECT_EQ(cbuf.front(), value_type());
    EXPECT_EQ(cbuf.back(), value_type());
}

TEST(CircularBufferTest, ComparingTwoBuffers) {
    CircularBuffer cbuf(2);
    cbuf.push_back('A');
    cbuf.push_back('B');
    CircularBuffer cbuf2(2);
    cbuf2.push_back('A');
    cbuf2.push_back('B');
    EXPECT_TRUE(cbuf == cbuf2);

    CircularBuffer cbuf3(2);
    cbuf3.push_back('B');
    cbuf3.push_back('A');
    EXPECT_FALSE(cbuf == cbuf3);

    CircularBuffer cbuf4(4);
    cbuf4.push_back('A');
    cbuf4.push_back('B');
    cbuf4.push_back('C');
    EXPECT_TRUE(cbuf != cbuf4);
}
