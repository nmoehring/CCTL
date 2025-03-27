#include <iostream>
#include "CCTL.hpp"

int main()
{
    CCTL::Valueable<int> test1{};
    std::cout << test1() << std::endl;
}