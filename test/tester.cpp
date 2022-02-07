#include <iostream>
#include "../rrps/include/rrps.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include <random>

struct BaseObj
{
    int a;
    double d;
};

class RandInt
{
private:
    int min{0};
    int max{10};
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> distr{min, max};

public:
    RandInt(int min, int max) : min(min), max(max)
    {
        distr = std::uniform_int_distribution<>{min, max};
    }

    int operator()() { return distr(gen); }
};

struct Com1 : public rrps::IServiceProvider<BaseObj, BaseObj>
{
private:
    std::string id{"com1"};
    BaseObj arg{1, 5.5};
    std::unordered_map<std::string, rrps::SubCallback<BaseObj>> sub_callback_list;
    RandInt randInt{0, 3};
    std::mutex arg_mutex;

private:
public:
    void operator()()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(randInt()));
        arg.a++;
        arg.d++;
        for (auto &sub : sub_callback_list)
        {
            sub.second(arg);
        }
    }

    void submit_publish_and_response_services(std::shared_ptr<rrps::IProvideServiceRegistration<BaseObj, BaseObj>> provide_registrator)
    {
        provide_registrator->add_publish_service_register_callback("com1_arg_updated", [&](const std::string &subscriber_id, std::function<void(BaseObj)> sub_callback)
                                                                   {
            if (sub_callback) {
                sub_callback_list[subscriber_id] = sub_callback;
            }
            else {
                std::cout << "ERROR: subscriber_id want to subscribe with empty function!!!" << std::endl;
            } });

        provide_registrator->add_response_service_register_callback("com1_get_arg", [&](const std::string &requester_id)
                                                                    { return [&](BaseObj a)
                                                                      { const std::lock_guard<std::mutex> lock(arg_mutex); return arg; }; });
    }
};

struct Com2 : public rrps::IServiceRequester<BaseObj, BaseObj>
{
private:
    std::string id{"com2"};
    RandInt randInt{0, 3};
    rrps::ResponseCallback<BaseObj, BaseObj> com1_get_arg;

public:
    void operator()()
    {
        // std::this_thread::sleep_for(std::chrono::seconds(randInt()));
        if (com1_get_arg)
        {
            auto a = com1_get_arg(BaseObj());
            std::cout << "Arg requested: a:" << a.a << ", b: " << a.d << std::endl;
        }
    }

    void submit_subscribe_and_requests_services(std::shared_ptr<rrps::IRequestServiceRegistration<BaseObj, BaseObj>> request_registrator)
    {
        request_registrator->register_subscriber(id, "com1_arg_updated", [](BaseObj arg)
                                                 { std::cout << "Arg updated: a:" << arg.a << ", b: " << arg.d << std::endl; });

        com1_get_arg = request_registrator->get_response_service(id, "com1_get_arg");
    }
};

int main(int argc, char const *argv[])
{
    auto rrps_service_mgr = std::make_shared<rrps::ReqResPubSubManager<BaseObj, BaseObj>>();
    Com1 com1;
    Com2 com2;
    try
    {
        if (std::is_base_of<rrps::IServiceProvider<BaseObj, BaseObj>, Com1>::value)
            com1.submit_publish_and_response_services(rrps_service_mgr);

        if (std::is_base_of<rrps::IServiceRequester<BaseObj, BaseObj>, Com2>::value)
            com2.submit_subscribe_and_requests_services(rrps_service_mgr);

        for (auto i = 0; i < 5; ++i)
        {
            com1();
            com2();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}