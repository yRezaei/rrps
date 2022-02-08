#ifndef FF8826D8_7A03_4ACF_B524_3CB03B1E7982
#define FF8826D8_7A03_4ACF_B524_3CB03B1E7982

/*
    prs: Request-Respons & Publisher-Subscriber
*/
namespace prs
{
    template <class T_return, class T_arg>
    class IReceiveServiceRegistrator
    {
    public:
        virtual ~IReceiveServiceRegistrator() {}
        virtual ResponseCallback<T_return, T_arg> get_response_service(const std::string &resquester_id, const std::string &response_service_id) = 0;
        virtual bool register_subscriber(const std::string &subscriber_id, const std::string &pub_service_id, SubCallback<T_arg> sub_callback) = 0;
    };
}

#endif /* FF8826D8_7A03_4ACF_B524_3CB03B1E7982 */
