#pragma once

#include <atomic>
#include <vector>

template<typename T>
class LockFreeQueue
{
private:

    std::vector<T> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    size_t capacity;

public:

    LockFreeQueue(size_t size)
        : buffer(size), head(0), tail(0), capacity(size) {}

    bool enqueue(const T& item)
    {
        size_t currentTail = tail.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % capacity;

        if (nextTail == head.load(std::memory_order_acquire))
            return false; // queue full

        buffer[currentTail] = item;

        tail.store(nextTail, std::memory_order_release);

        return true;
    }

    bool dequeue(T& item)
    {
        size_t currentHead = head.load(std::memory_order_relaxed);

        if (currentHead == tail.load(std::memory_order_acquire))
            return false; // queue empty

        item = buffer[currentHead];

        head.store((currentHead + 1) % capacity,
                   std::memory_order_release);

        return true;
    }
};