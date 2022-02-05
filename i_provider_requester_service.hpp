#ifndef RRPS_I_PROVIDER_REQUESTER_SERVICE_HPP_
#define RRPS_I_PROVIDER_REQUESTER_SERVICE_HPP_

#include <memory>

namespace rrps
{
    template <class T_return, class T_arg>
    class IServiceProvider
    {
    public:
        virtual ~IServiceProvider() {}
        virtual void submit_publish_and_response_services(std::shared_ptr<rrps::IProvideServiceRegistration<T_return, T_arg>> provider_registrator) = 0;
    };

    template <class T_return, class T_arg>
    class IServiceRequester
    {
    public:
        virtual ~IServiceRequester() {}
        virtual void submit_subscribe_and_requests_services(std::shared_ptr<rrps::IRequestServiceRegistration<T_return, T_arg>> request_registrator) = 0;
    };
}

#endif // end of RRPS_I_PROVIDER_REQUESTER_SERVICE_HPP_