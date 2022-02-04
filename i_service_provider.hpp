#ifndef RRPS_I_SERVICE_PROVIDER_HPP_
#define RRPS_I_SERVICE_PROVIDER_HPP_

#include "basic_types.hpp"

namespace rrps
{
    template <class T_return, class T_arg>
    class IServiceProvider
    {
    public:
    public:
        virtual ~IServiceProvider() {}
        virtual bool add_response_service_register_callback(const std::string &res_service_id, ResponseServiceRegisterCallback<T_return, T_arg> res_register_callback) = 0;
        virtual bool add_publish_service_register_callback(const std::string &pub_service_id, SubscribeServiceRegisterCallback<T_arg> sub_register_callback) = 0;
    };
}

#endif // end of RRPS_I_SERVICE_PROVIDER_HPP_