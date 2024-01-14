/**
* @file main.cpp
* @author Makaka
* @date 2023-12-29
* @brief 两人争夺同一个黑板(string)
*/

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

class ThreadSafeString {
private:
    std::string data;
    mutable std::mutex mutex;

public:
    ThreadSafeString() {}

    void append(const std::string& text) {
        std::lock_guard<std::mutex> lock(mutex);
        data += text;
    }

    // 重载 += 运算符，使其线程安全
    ThreadSafeString& operator+=(const std::string& text) {
        std::lock_guard<std::mutex> lock(mutex);
        data += text;
        return *this;
    }

    // 获取字符串的副本
    std::string get() const {
        std::lock_guard<std::mutex> lock(mutex);
        return data;
    }

    // 友元函数，重载输出运算符
    friend std::ostream& operator<<(std::ostream& os, const ThreadSafeString& str) {
        os << str.data;
        return os;
    }
    
};

void maWrite(ThreadSafeString &board){
    board += "My";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " name";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " is";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " Makaka";
}

void zhaoWrite(ThreadSafeString &board){
    board += "My";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " name";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " is";
    this_thread::sleep_for(std::chrono::milliseconds(10));
    board += " ZhaoXiao";
}


int main(){
    ThreadSafeString board;
    thread MaWrite(maWrite, std::ref(board));
    thread ZhaoWrite(zhaoWrite, std::ref(board));
    MaWrite.join();
    ZhaoWrite.join();
    std::cout << board << std::endl;
    return 0;
}