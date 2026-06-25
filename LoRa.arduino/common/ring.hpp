#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <cstddef>

template <typename T, size_t N>
class RingBuffer
{
    static_assert(N > 0, "RingBuffer size must be greater than zero");
    static_assert((N & (N - 1)) == 0, "RingBuffer size must be a power of two");
public:
    RingBuffer() = default;

    bool push(const T &item)
    {
        if (isFull())
            return false; // Buffer is full

        buffer[head] = item;
        head = (head + 1) & (N - 1);

        return true;
    }

    bool pop(T &item)
    {
        if (isEmpty())
            return false; // Buffer is empty

        item = buffer[tail];
        tail = (tail + 1) & (N - 1);

        return true;
    }

    bool isEmpty() const
    {
        return (!isFull() && (head == tail));
    }

    bool isFull() const
    {
        return ((head + 1) & (N - 1)) == tail;
    }

    size_t size() const
    {
        if (isFull())
            return N;

        if (head >= tail)
            return head - tail;
        else
            return N + head - tail;
    }

private:    
    T buffer[N];
    size_t head = 0;
    size_t tail = 0;
};

#endif // RING_BUFFER_HPP