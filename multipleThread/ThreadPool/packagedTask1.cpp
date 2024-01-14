#include <iostream>
#include <future>
#include <thread>

// 一个简单的函数，计算平方
int square(int x) {
    return x * x;
}

int main() {
    // 创建一个 std::packaged_task 对象，封装 square 函数
    // auto task = std::make_shared<std::packaged_task<int(int)>>(square);
    auto task = std::make_shared<std::packaged_task<decltype(square(0)) (int)>> (std::move(square));

    // 获取与任务相关联的 future
    std::future<decltype(square(0))> result = task->get_future();
    // std::future<int> result = task->get_future();

    // 启动线程执行任务
    std::thread thread([task] {
        (*task)(5);  // 调用 square 函数，计算 5 的平方
    });

    // 在主线程中获取结果
    std::cout << "The square of 5 is " << result.get() << std::endl;

    // 等待线程完成
    thread.join();

    return 0;
}