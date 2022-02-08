#ifndef DF955289_E34C_4A5D_B85B_9AD0ED647580
#define DF955289_E34C_4A5D_B85B_9AD0ED647580

#include <iostream>
#include <unordered_map>
#include <mutex>

#include "i_provide_service_registrator.hpp"
#include "i_request_service_registrator.hpp"

/*
    rrps: Request-Respons & Publisher-Subscriber
*/
namespace rrps
{
    template <class T_return, class T_arg>
    class ReqResPubSubManager : public IProvideServiceRegistrator<T_return, T_arg>, public IReceiveServiceRegistrator<T_return, T_arg>
    {
    private:
        std::mutex reg_mutex;
        std::unordered_map<std::string, ResponseServiceRegisterCallback<T_return, T_arg> > res_service_register_callback_list;
        std::unordered_map<std::string, SubscribeServiceRegisterCallback<T_arg> > sub_service_register_callback_list;

    public:
        bool add_response_service_register_callback(const std::string &res_service_id, ResponseServiceRegisterCallback<T_return, T_arg> res_register_callback) override
        {
            const std::lock_guard<std::mutex> lock(reg_mutex);
            if (res_register_callback)
            {
                std::cout << "The service id \"" << res_service_id << "\" registered." << std::endl;
                res_service_register_callback_list[res_service_id] = res_register_callback;
                return true;
            }
            else
            {
                std::cout << "ERROR: " << __FUNCTION__ << " #"<< __LINE__ << " no such a response service exist: " << res_service_id << std::endl;
                return false;
            }
        }

        ResponseCallback<T_return, T_arg> get_response_service(const std::string& resquester_id, const std::string& response_service_id) override
        {
            const std::lock_guard<std::mutex> lock(reg_mutex);
            if (does_service_exists(response_service_id))
            {
                std::cout << resquester_id << " requested the \"" << response_service_id << "\" service." << std::endl;
                return res_service_register_callback_list[response_service_id](resquester_id);
            }
            else
            {
                ResponseCallback<T_return, T_arg> tmp;
                std::cout << "ERROR: " << __FUNCTION__ << " #"<< __LINE__ << " no such a ReqRes service exist: " << response_service_id << std::endl;
                return tmp;
            }
        }

        bool does_service_exists(const std::string& service_id) 
        {
            return (res_service_register_callback_list.find(service_id) != res_service_register_callback_list.end() ? true : false);
        }

        bool add_publish_service_register_callback(const std::string &pub_service_id, SubscribeServiceRegisterCallback<T_arg> sub_register_callback) override
        {
            const std::lock_guard<std::mutex> lock(reg_mutex);
            if (sub_register_callback)
            {
                std::cout << "Publish service \"" << pub_service_id << "\" is registered." << std::endl;
                sub_service_register_callback_list[pub_service_id] = sub_register_callback;
                return true;
            }
            else
            {
                std::cout << "ERROR: no such a Publish service exist: " << pub_service_id << std::endl;
                return false;
            }
        }

        bool register_subscriber(const std::string& subscriber_id, const std::string& pub_service_id, SubCallback<T_arg> sub_callback) override
        {
            const std::lock_guard<std::mutex> lock(reg_mutex);
            if (sub_service_register_callback_list.find(pub_service_id) == sub_service_register_callback_list.end())
            {
                std::cout << "ERROR: " << __FUNCTION__ << " #"<< __LINE__ << " No such a \""<< pub_service_id << "\" service exist " << std::endl;
                return false;
            }
            else
            {
                if (sub_callback)
                {
                    std::cout << subscriber_id << " subscribed for publish service \"" << pub_service_id << "\"." << std::endl;
                    sub_service_register_callback_list[pub_service_id](subscriber_id, sub_callback);
                    return true;
                }
                else
                {
                    std::cout << "ERROR: " << __FUNCTION__ << " #"<< __LINE__ << " Empty callback is passed."  << std::endl;
                    return false;
                }
            }
        }
    };
}

#endif /* DF955289_E34C_4A5D_B85B_9AD0ED647580 */
