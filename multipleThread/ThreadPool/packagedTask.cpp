#include <iostream>
#include <future>

int main(){
    // 类模板 std::packaged_task 包装任何可调用 (Callable) 目标（函数、 lambda 表达式、 bind 表达式或其他函数对象），使得能异步调用它。
    std::packaged_task<int(int, int)> sumTask([=] (int a, int b){
        return a + b;
    });
    // 其返回值或所抛异常被存储于能通过 std::future 对象访问的共享状态中。future供访问异步操作结果的机制：
    std::future<int> sumResult = sumTask.get_future(); 
    sumTask(100,200);

    std::cout << "sumResult: " << sumResult.get() << std::endl;

}