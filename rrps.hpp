#ifndef REQUEST_RESPONSE_PUBLISHER_SUBSCRIBER_HPP_
#define REQUEST_REPONSE_PUBLISHER_SUBSCRIBER_HPP_

#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>

#include "i_service_provider.hpp"
#include "i_service_requester.hpp"

namespace rrps
{
    template <class T_return, class T_arg>
    class ReqResPubSubManager : public IServiceProvider<T_return, T_arg>, public IServiceRequester<T_return, T_arg>
    {
    private:
        std::unordered_map<std::string, ResponseServiceRegisterCallback<T_return, T_arg> > res_service_register_callback_list;
        std::unordered_map<std::string, SubscribeServiceRegisterCallback<T_arg> > sub_service_register_callback_list;

    public:
        bool add_response_service_register_callback(const std::string &res_service_id, ResponseServiceRegisterCallback<T_return, T_arg> res_register_callback) override
        {
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

#endif // End of REQUEST_REPONSE_PUBLISHER_SUBSCRIBER_H_