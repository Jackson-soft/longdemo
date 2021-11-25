#pragma once

//最小堆
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <vector>

namespace uranus::utils {
template<typename T>
class MinHeap
{
public:
    MinHeap() : MinHeap(30) {}

    //这是做一下预分配内存
    explicit MinHeap(int capacity) {
        array_.reserve(capacity);
    }

    ~MinHeap() {
        array_.clear();
    }

    //插入新元素
    auto Push(const T &elem) -> bool {
        //独享写锁
        std::unique_lock<std::mutex> lock(mutex_);
        array_.emplace_back(elem);
        up(array_.size() - 1);
        return true;
    }

    //获取堆顶元素
    auto Pop() -> T {
        //共享读锁
        std::shared_lock<std::mutex> lock(mutex_);
        if (!array_.empty()) {
            //出堆的数据
            T mMin = array_.front();
            //最后一个数据放到第一个根上面
            std::swap(array_.front(), array_.back());
            array_.pop_back();
            down(0);
            return std::move(mMin);
        }
        //这里有一个警告，目前还没想到怎么去除
        // return xxx;
    }

    auto IsEmpty() -> bool {
        return array_.empty();
    }

    auto Size() -> size_t {
        return array_.size();
    }

    auto operator[](int i) -> T & {
        return array_[i];
    }

private:
    //上浮
    void up(int index) {
        int n = 0;
        int p = 0;
        while (index > 0) {
            n = index % 2;
            if (n == 0) {
                p = (index - 2) / 2;
            } else {
                p = (index - 1) / 2;
            }
            if (array_[index] < array_[p]) {
                std::swap(array_[index], array_[p]);
                index = p;
            } else {
                break;
            }
        }
    }

    //下沉
    void down(int index) {
        int size = array_.size();
        while (2 * index + 1 <= size - 1) {
            if (array_[index] > array_[2 * index + 1]) {
                std::swap(array_[index], array_[2 * index + 1]);
                if (2 * index + 2 <= size - 1 && array_[index] > array_[2 * index + 2]) {
                    std::swap(array_[index], array_[2 * index + 2]);
                }
                index = 2 * index + 1;
            } else {
                break;
            }
        }
    }

    std::vector<T>           array_;
    mutable std::timed_mutex mutex_;  // mutable声明可变数据成员
};
}  // namespace uranus::utils