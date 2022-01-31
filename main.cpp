#include <iostream>
#include "rrps.hpp"
struct ArgumentType {
    int a;
    double d;
};

int main(int argc, char const *argv[])
{
    rrps::ReqResPubSubManager<ArgumentType, ArgumentType> service_mgr;

    std::cout << "Hello RRPS" << std::endl;
    return 0;
}
