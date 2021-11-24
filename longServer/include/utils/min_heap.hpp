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
        mArray.reserve(capacity);
    }

    ~MinHeap() {
        mArray.clear();
    }

    //插入新元素
    auto push(const T &elem) -> bool {
        //独享写锁
        std::unique_lock<std::mutex> lock(mMutex);
        mArray.emplace_back(elem);
        up(mArray.size() - 1);
        return true;
    }

    //获取堆顶元素
    T pop() {
        //共享读锁
        std::shared_lock<std::mutex> lock(mMutex);
        if (!mArray.empty()) {
            //出堆的数据
            T mMin = mArray.front();
            //最后一个数据放到第一个根上面
            std::swap(mArray.front(), mArray.back());
            mArray.pop_back();
            down(0);
            return std::move(mMin);
        }
        //这里有一个警告，目前还没想到怎么去除
        // return xxx;
    }

    auto isEmpty() -> bool {
        return mArray.empty();
    }

    auto size() -> size_t {
        return mArray.size();
    }

    auto operator[](int i) -> T & {
        return mArray[i];
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
            if (mArray[index] < mArray[p]) {
                std::swap(mArray[index], mArray[p]);
                index = p;
            } else {
                break;
            }
        }
    }

    //下沉
    void down(int index) {
        int size = mArray.size();
        while (2 * index + 1 <= size - 1) {
            if (mArray[index] > mArray[2 * index + 1]) {
                std::swap(mArray[index], mArray[2 * index + 1]);
                if (2 * index + 2 <= size - 1 && mArray[index] > mArray[2 * index + 2]) {
                    std::swap(mArray[index], mArray[2 * index + 2]);
                }
                index = 2 * index + 1;
            } else {
                break;
            }
        }
    }

    std::vector<T>           mArray;
    // mutable声明可变数据成员
    mutable std::timed_mutex mMutex;
};
}  // namespace uranus::utils