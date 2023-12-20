#include "function.hpp"
#include <iostream>

int globalVar1 = 100;

void fun1(){
    std::cout << "Hello World!" << std::endl;
    std::cout << "globalVar1 : " << globalVar1 << std::endl;
}