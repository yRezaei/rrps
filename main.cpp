#include <iostream>
#include "test.hpp"
#include <type_traits>

int main(int argc, char const *argv[])
{
    auto rrps_service_mgr = std::make_shared<rrps::ReqResPubSubManager<BaseObj, BaseObj>>();
    Com1 com1;
    Com2 com2;
try
{
    if(std::is_base_of<rrps::IServiceProvider<BaseObj, BaseObj>, Com1>::value)
        com1.submit_publish_and_response_services(rrps_service_mgr);

    if(std::is_base_of<rrps::IServiceRequester<BaseObj, BaseObj>, Com2>::value)
        com2.submit_subscribe_and_requests_services(rrps_service_mgr);

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
