#include <iostream>
#include "test.hpp"

int main(int argc, char const *argv[])
{
    auto service_mgr = std::make_shared<rrps::ReqResPubSubManager<ArgumentType, ArgumentType>>();
    Com1 com1;
    Com2 com2;
try
{
    com1.submit_publish_and_response_sercices(service_mgr);
    com2.submit_publish_and_response_sercices(service_mgr);

    com1.submit_subscribe_and_requests_services(service_mgr);
    com2.submit_subscribe_and_requests_services(service_mgr);

    for(auto i = 0; i < 5; ++i) {
       com1();
       com2();
    }
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}

    
    return 0;
}
