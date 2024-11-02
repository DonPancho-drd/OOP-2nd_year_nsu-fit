#include <iostream>
#include <stdexcept>
#include "circular_buf.h"

typedef char value_type;

CircularBuffer::CircularBuffer() : buffer(nullptr), // конструктор для буфера нулевого размера
                                   current_size(0),
                                   capacity_b(0),
                                   front_index(0),
                                   back_index(0)
{
}

CircularBuffer::~CircularBuffer()
{
    delete[] buffer;
}

CircularBuffer::CircularBuffer(const CircularBuffer &cb)
{                                           // конструктор копирования константого обьекта
    buffer = new value_type[cb.capacity_b]; // выделяем новое место в памяти
    current_size = cb.current_size;
    capacity_b = cb.capacity_b;
    front_index = cb.front_index;
    back_index = cb.back_index;
    for (int i = 0; i < current_size; i++)
    {
        buffer[i] = cb.buffer[i];
    }
}

// Конструирует буфер заданной ёмкости

CircularBuffer::CircularBuffer(int capacity)
{
    buffer = new value_type[capacity];
    current_size = 0;
    capacity_b = capacity;
    front_index = 0;
    back_index = 0;
}

// Конструирует буфер заданной ёмкости, целиком заполняет его элементом elem

CircularBuffer::CircularBuffer(int capacity, const value_type &elem)
{
    buffer = new value_type[capacity];
    current_size = capacity;
    capacity_b = capacity;
    front_index = 0;
    back_index = 0;
    for (int i = 0; i < capacity; i++)
    {
        buffer[i] = elem;
    }
}

value_type &CircularBuffer::operator[](int i)
{
    return buffer[(front_index + i) % capacity_b]; // индекс по модулю ёмкости(capacity)
}

const value_type &CircularBuffer::operator[](int i) const
{
    return buffer[(front_index + i) % capacity_b]; // индекс по модулю ёмкости(capacity)
}

// Доступ по индексу. Методы бросают исключение в случае неверного индекса.
value_type &CircularBuffer::at(int i)
{
    if (i < 0 || i >= current_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return buffer[(front_index + i) % capacity_b];
}

const value_type &CircularBuffer::at(int i) const
{
    if (i < 0 || i >= current_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return buffer[(front_index + i) % capacity_b];
}

value_type &CircularBuffer::front() // ссылка на первый элемент
{
    return buffer[front_index];
}

value_type &CircularBuffer::back() // ссылка на последний элемент
{
    return buffer[back_index];
}

const value_type &CircularBuffer::front() const
{
    return buffer[front_index];
}

const value_type &CircularBuffer::back() const
{
    return buffer[back_index];
}

value_type *CircularBuffer::linearize()
{
    if (!is_linearized())
    {
        value_type *new_buffer = new value_type[capacity_b];
        for (int i = 0; i < current_size; i++)
        {
            new_buffer[i] = buffer[(front_index + i) % capacity_b];
        }
        delete[] buffer;
        buffer = new_buffer;
        front_index = 0;
        back_index = current_size - 1;
    }
    return buffer;
}

bool CircularBuffer::is_linearized() const
{
    return (front_index == 0);
}

// rotate - сдвигает буфер так, что по нулевому индексу окажется элемент
// с индексом new_begin.
void CircularBuffer::rotate(int new_begin)
{
    if (new_begin < 0 || new_begin >= current_size)
    {
        throw std::out_of_range("Index out of range");
    }

    if (current_size < capacity_b)
    {
        for (int i = 0; i < new_begin; i++)
        {
            buffer[(back_index + i + 1) % capacity_b] = buffer[(front_index + i) % capacity_b];
        }
    }
    front_index = (front_index + new_begin) % capacity_b;
    back_index = (back_index + new_begin) % capacity_b;
}

int CircularBuffer::size() const
{
    return current_size;
}

bool CircularBuffer::empty() const
{
    return (current_size == 0);
}

// true, если size() == capacity().
bool CircularBuffer::full() const
{
    return (current_size == capacity_b);
}
// Количество свободных ячеек в буфере.
int CircularBuffer::reserve() const
{
    return (capacity_b - current_size);
}
int CircularBuffer::capacity() const
{
    return capacity_b;
}

void CircularBuffer::set_capacity(int new_capacity)
{
    if (new_capacity <= 0)
    {
        throw std::invalid_argument("Capacity must be positive");
    }

    value_type *new_buffer = new value_type[new_capacity];

    for (int i = 0; i < current_size; i++)
    {
        new_buffer[i] = buffer[(front_index + i) % capacity_b];
    }

    delete[] buffer;
    buffer = new_buffer;

    capacity_b = new_capacity;
    front_index = 0;
    back_index = current_size - 1;
}

void CircularBuffer::resize(int new_size, const value_type &item = value_type())
{
    if (new_size < 0)
    {
        throw std::invalid_argument("New size must be positive");
    }

    if (new_size == 0)
    {
        clear();
        return;
    }

    if (new_size < current_size)
    {
        current_size = new_size;
        return;
    }

    if (new_size > capacity_b)
    {
        set_capacity(new_size);
    }

    if (new_size > current_size)
    {
        for (int i = 1; i <= (new_size - current_size); i++)
        {
            buffer[(back_index + i) % capacity_b] = item;
        }
    }
    current_size = new_size;
}

CircularBuffer &CircularBuffer::operator=(const CircularBuffer &cb)
{
    if (this == &cb){
        return (*this);
        }

    if ((*this) != cb){
        delete[] buffer;
        }
    capacity_b = cb.capacity_b;
    current_size = cb.current_size;
    front_index = cb.front_index;
    back_index = cb.back_index;
    value_type *buffer = new value_type[capacity_b];
    for (int i = 0; i < current_size; i++)
    {
        buffer[(front_index + i) % capacity_b] = cb.buffer[(front_index + i) % capacity_b];
    }
    return (*this);
}

void CircularBuffer::swap(CircularBuffer &cb)
{
    if (this == &cb)
    {
        return;
    }
    std::swap(buffer, cb.buffer);
    std::swap(capacity_b, cb.capacity_b);
    std::swap(current_size, cb.current_size);
    std::swap(front_index, cb.front_index);
    std::swap(back_index, cb.back_index);
}

void CircularBuffer::push_back(const value_type &item = value_type())
{
    back_index = (back_index + 1) % capacity_b;
    buffer[back_index] = item; // buffer[back_index + 1]
    if (front_index == back_index)
    {
        front_index = (front_index + 1) % capacity_b;
    }
    else
    {
        current_size++;
    }
}
void CircularBuffer::push_front(const value_type &item = value_type())
{
    front_index = (front_index - 1 + capacity_b) % capacity_b;
    buffer[front_index] = item;
    if (front_index == back_index)
    {
        back_index = (back_index - 1 + capacity_b) % capacity_b;
    }
    else
    {
        current_size++;
    }
}
void CircularBuffer::pop_back()
{
    back_index = (back_index - 1 + capacity_b) % capacity_b;
    current_size--;
}
void CircularBuffer::pop_front()
{
    front_index = (front_index + 1) % capacity_b;
    current_size--;
}


void CircularBuffer::insert(int pos, const value_type &item = value_type())
{
    if (pos < 0 || pos > current_size)
    {
        throw std::out_of_range("Index out of range");
    }

    if (current_size == capacity_b)
    {
        throw std::overflow_error("Buffer is full");
    }

    if (current_size < capacity_b)
    {
        for (int i = current_size; i > pos; i--)
        {
            buffer[(front_index + i) % capacity_b] = buffer[(front_index + i - 1) % capacity_b];
        }
    }
    buffer[(front_index + pos) % capacity_b] = item;
    current_size++;
    back_index = (back_index + 1) % capacity_b;
}
// Удаляет элементы из буфера в интервале [first, last).
void CircularBuffer::erase(int first, int last)
{
    if (first < 0 || first > last || last > current_size)
    {
        throw std::out_of_range("Index out of range");
    }
    int index_for_shift = (front_index + last) % capacity_b;

    for (int i = first; index_for_shift != (back_index + 1) % capacity_b; i++)
    {
        buffer[i] = buffer[index_for_shift];
        index_for_shift = (index_for_shift + 1) % capacity_b;
    }
    current_size -= (last - first);

    if (current_size == 0)
    {
        front_index = 0;
        back_index = 0;
    }
    else
    {
        back_index = (front_index + current_size - 1) % capacity_b;
    }
}
void CircularBuffer::clear()
{
    for (int i = 0; i < current_size; i++)
    {
        buffer[i] = value_type();
    }
    current_size = 0;
    front_index = 0;
    back_index = 0;
}

bool operator==(const CircularBuffer &a, const CircularBuffer &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}
bool operator!=(const CircularBuffer &a, const CircularBuffer &b)
{
    return !(a == b);
}
