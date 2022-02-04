#ifndef RRPS_I_SERVICE_REQUESTER_HPP_
#define RRPS_I_SERVICE_REQUESTER_HPP_

namespace rrps
{
    template <class T_return, class T_arg>
    class IServiceRequester
    {
    public:
        virtual ~IServiceRequester() {}
        virtual ResponseCallback<T_return, T_arg> get_response_service(const std::string& resquester_id, const std::string& response_service_id) = 0; 
        virtual bool register_subscriber(const std::string& subscriber_id, const std::string& pub_service_id, SubCallback<T_arg> sub_callback) = 0;
    };
}

#endif // end of RRPS_I_SERVICE_REQUESTER_HPP_