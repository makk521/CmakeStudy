#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>

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

    template<class T>
    auto enqueue(T task)->std::future<decltype(task())>{ // decltype(task())用于获取task()表达式的返回类型。
        // 将task存入任务队列中
        // make_shared()函数创建一个std::shared_ptr对象，该对象包含一个指向Task对象的指针。他是线程安全的，它允许std::packaged_task在多个线程之间安全地共享。
        // packaged_task允许你将一个可调用对象（函数、函数对象或 Lambda 表达式）包装成一个异步任务，使得能异步调用它,并提供一种获取任务结果的机制。
        // std::packaged_task是一个可调用对象包装器，它包装了一个可调用对象（如函数、函数对象或lambda表达式）。当std::packaged_task被调用时，它会调用它包装的可调用对象，并将结果保存起来。然后，你可以通过与std::packaged_task关联的std::future对象获取这个结果。
        auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>> (std::move(task)); 
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mTasks.emplace([=]{ // 这个lambda表达式捕获了当前作用域中的所有变量（通过[=]），并在被调用时执行(*wrapper)()。
                (*wrapper)();
            }); // 使用std::move(task)，emplace方法将会接收到一个右值。在这种情况下，emplace方法可以直接利用task的资源，创建一个新的Task对象，而不需要复制task的数据。这样可以避免复制操作的开销，提高代码的效率。
        }
        mEnventVar.notify_one(); // 唤醒一个在mEnventVar上等待的线程。这样，如果线程池中有空闲线程，那么这个线程就会从任务队列中取出一个任务并执行。
        return wrapper->get_future();
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
                        std::unique_lock<std::mutex> lock(mMutex); // 对于一个线程，先获取任务队列可访问权限，即保证只有一个线程在操作任务队列
                        mEnventVar.wait(lock, [=]{return mStop || !mTasks.empty();}); // true时继续执行，false时阻塞  控制mStop信号停或任务队列不为空时继续执行
        
                        if(mStop && mTasks.empty()){
                            break; // 如果线程池已经停止运行，且任务队列为空，那么线程就会退出循环，从而结束运行。
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
    auto f1 = pool.enqueue([=]{
        std::cout << "1 is running." << std::endl;
        return 1;
    });
    auto f2 = pool.enqueue([=]{
        std::cout << "2 is running." << std::endl;
        return 2;
    });
    auto f3 = pool.enqueue([=]{
        std::cout << "3 is running." << std::endl;
        return 3;
    });
    std::cout << f1.get() << std::endl;
    std::cout << f2.get() << std::endl;
    std::cout << f3.get() << std::endl;
    return 0;
}
