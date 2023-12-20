#include <iostream>
#include "function.hpp"

int main(){
    fun1();
    std::cout << "main函数引用globalVar1:" << globalVar1 << std::endl;
    return 0;
}