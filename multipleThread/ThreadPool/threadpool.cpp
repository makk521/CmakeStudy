#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class ThreadsPool 
{
public:
    using Task = std::function<void()>;

    explicit ThreadsPool(std::size_t numThreads){
        start(numThreads);
    }

    ~ThreadsPool() noexcept{
        stop();
    }

    void enqueue(Task task){
        // 将task存入任务队列中
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mTasks.emplace(std::move(task)); // 使用std::move(task)，emplace方法将会接收到一个右值。在这种情况下，emplace方法可以直接利用task的资源，创建一个新的Task对象，而不需要复制task的数据。这样可以避免复制操作的开销，提高代码的效率。
        }
        mEnventVar.notify_one(); // 唤醒一个在mEnventVar上等待的线程。这样，如果线程池中有空闲线程，那么这个线程就会从任务队列中取出一个任务并执行。
    }
private:
    std::vector<std::thread> mThreads;
    std::condition_variable mEnventVar;
    std::mutex mMutex;
    bool mStop = false;
    std::queue<Task> mTasks;

    void start(std::size_t numThreads){
        for(auto i = 0u; i<numThreads; ++i){ // 0u: unsigned int size_t: 无符号整型   ++i理论上执行效率高于i++
            mThreads.emplace_back([=]{ // [=]表示该lambda表达式以值捕获所有可用变量。
            // emplace_back()函数接受一个lambda表达式作为参数，然后调用std::thread的构造函数，将这个lambda表达式作为参数传递给构造函数，创建一个新的std::thread对象，并将这个对象添加到threads向量的末尾。
                while (true)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(mMutex);
                        mEnventVar.wait(lock, [=]{return mStop || !mTasks.empty();}); // true时继续执行，false时阻塞
        
                        if(mStop && mTasks.empty()){
                            break;
                        }

                        task = std::move(mTasks.front()); // 从任务队列中取出一个任务
                        mTasks.pop(); // 从任务队列中移除这个任务
                    }
                    task(); // 执行任务
                }
                
            });
        }
    }
    void stop(){
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mStop = true;
        }
        mEnventVar.notify_all(); // 唤醒所有在mEnventVar上等待的线程。这样，所有的线程都会检查mStop的值，如果mStop为true，那么线程会退出其循环，从而结束运行。

        for(auto &thread : mThreads){ // 确保在stop方法返回之前，线程池中的所有线程都已经结束运行。
            thread.join();
        }
    }
};


int main(){
    ThreadsPool pool(4);
    pool.enqueue([=]{
        std::cout << "1 is running." << std::endl;
    });
    pool.enqueue([=]{
        std::cout << "2 is running." << std::endl;
    });
    pool.enqueue([=]{
        std::cout << "3 is running." << std::endl;
    });
    return 0;
}