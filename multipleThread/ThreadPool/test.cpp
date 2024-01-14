#include <iostream>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>

class ThreadPool{
public:
    using Task = std::function<void()>;

    ThreadPool(int numThreads){
        start(numThreads);
    }

    ~ThreadPool(){
        stop();
    }
private:
    std::mutex mutex_;
    std::condition_variable cond_;
    std::vector<std::thread> threads_;
    bool stop_;

    void start(int numThreads){
        for(int i=0; i<numThreads; ++i){
            threads_.emplace_back([=]{
                while(true){
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        cond_.wait(lock, [=]{return stop_;})
                    }
                }
            })
        }
    }
    void stop(){

    }
};

int main(){
    ThreadPool pool(4);

    return 0;
}