#ifndef BC952FAB_3052_49E2_B66A_514B3E4DAC41
#define BC952FAB_3052_49E2_B66A_514B3E4DAC41

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

#endif /* BC952FAB_3052_49E2_B66A_514B3E4DAC41 */
