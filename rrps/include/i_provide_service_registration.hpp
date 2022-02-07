#ifndef RRPS_I_PROVIDE_SERVICE_REGISTRATION_HPP_
#define RRPS_I_PROVIDE_SERVICE_REGISTRATION_HPP_

#include "basic_types.hpp"

namespace rrps
{
    template <class T_return, class T_arg>
    class IProvideServiceRegistration
    {
    public:
    public:
        virtual ~IProvideServiceRegistration() {}
        virtual bool add_response_service_register_callback(const std::string &res_service_id, ResponseServiceRegisterCallback<T_return, T_arg> res_register_callback) = 0;
        virtual bool add_publish_service_register_callback(const std::string &pub_service_id, SubscribeServiceRegisterCallback<T_arg> sub_register_callback) = 0;
    };
}

#endif // end of RRPS_I_PROVIDE_SERVICE_REGISTRATION_HPP_