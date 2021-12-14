#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

static const int DEFAULT_MAX_QUEUE_SIZE = 1000;

template<typename T>
class ThreadSafeQueue
{
private:
    int maxSize_;
    queue<T> queue_;
    mutex mutex_;
    condition_variable emptyCond_;
    condition_variable fullCond_;

public:
    ThreadSafeQueue(const int maxSize = DEFAULT_MAX_QUEUE_SIZE):maxSize_(maxSize)
    {}

    void Push(const T& t)
    {
        unique_lock<mutex> lock(mutex_);
        fullCond_.wait(lock, [this](){return queue_.size() < maxSize_;});
        queue_.push(t);
        emptyCond_.notify_one();
    }

    void Pop(T &t)
    {
        unique_lock<mutex> lock(mutex_);
        emptyCond_.wait(lock, [this] (){return !queue_.empty();});  //条件变量不满足，lock解锁，线程阻塞
        t = move(queue_.front());
        queue_.pop();

        fullCond_.notify_one();
    }

    int Size()
    {
        return queue_.size();
    }
};