#ifndef REQUEST_RESPONSE_PUBLISHER_SUBSCRIBER_H_
#define REQUEST_REPONSE_PUBLISHER_SUBSCRIBER_H_

#include <unordered_map>
#include <string>
#include <functional>

namespace rrps
{

template<typename ReturnType, typename ArgumentType>
class ReqResPubSubManager
{
    private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::function<void(ArgumentType)> > > sub_pub_func_list;
    std::unordered_map<std::string, std::function<ReturnType(ArgumentType)> > service_func_list;

    public:
    ReturnType request_service(const std::string& requester_id, const std::string& service_id, ArgumentType arg) {
        if(service_func_list.find(service_id) == service_func_list.end()) {
            return ReturnType();
        }
        else {
            return service_func_list[request_id].second(arg);
        }
    }

    void publish(const std::string& callback_id, ArgumentType arg) {
        if(sub_pub_func_list.find(callback_id) != sub_pub_func_list.end()) {
            for(auto& sub : sub_pub_func_list[callback_id]) {
                sub.second(arg);
            }
        }
    }

    void register_response_service(const std::string& service_id, std::function<ReturnType(ArgumentType)> service) {
        service_func_list[service_id] = service;
    }

    bool does_service_exists(const std::string& service_id) {
        return (service_func_list.find(service_id) != service_func_list.end() ? true : false);
    }

    bool register_subscriber(const std::string& sunscriber_id, const std::string& callback_id, std::function<void(ArgumentType)> callback) {
        if(sub_pub_func_list.find(callback_id) == sub_pub_func_list.end()) {
            return false;
        }
        else {
            sub_pub_func_list[callback_id].insert({sunscriber_id, callback});
        }
    }
};
}


#endif // End of REQUEST_REPONSE_PUBLISHER_SUBSCRIBER_H_