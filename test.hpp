#include <iostream>
#include "rrps.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include <random>

struct ArgumentType
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

using RRPSManager = rrps::ReqResPubSubManager<ArgumentType, ArgumentType>;

class IRRPSCallbackInit
{
public:
    virtual ~IRRPSCallbackInit() {}
    virtual void submit_publish_and_response_sercices(std::shared_ptr<rrps::IServiceProvider<ArgumentType, ArgumentType>> rrpsMGR) = 0;
    virtual void submit_subscribe_and_requests_services(std::shared_ptr<rrps::IServiceRequester<ArgumentType, ArgumentType>> rrpsMGR) = 0;
};

struct Com1 : public IRRPSCallbackInit
{
private:
    std::string id{"com1"};
    ArgumentType arg{1, 5.5};
    std::unordered_map<std::string, rrps::SubCallback<ArgumentType> > sub_callback_list;
    RandInt randInt{0, 3};

private:


public:
    void operator()()
    {
        std::this_thread::sleep_for(std::chrono::seconds(randInt()));
        arg.a++;
        arg.d++;
        for (auto &sub : sub_callback_list)
        {
            sub.second(arg);
        }
    }

    void submit_publish_and_response_sercices(std::shared_ptr<rrps::IServiceProvider<ArgumentType, ArgumentType>> rrpsMGR)
    {
        rrpsMGR->add_publish_service_register_callback("com1_arg_updated", [&](const std::string &subscriber_id, std::function<void(ArgumentType)> sub_callback)
        {
            if (sub_callback) {
                sub_callback_list[subscriber_id] = sub_callback;
            }
            else {
                std::cout << "ERROR: subscriber_id want to subscribe with empty function!!!" << std::endl;
            }
        });

        rrpsMGR->add_response_service_register_callback("com1_get_arg", [&](const std::string &requester_id) { return [&](ArgumentType a) { return arg; }; });
    }

    void submit_subscribe_and_requests_services(std::shared_ptr<rrps::IServiceRequester<ArgumentType, ArgumentType>> rrpsMGR)
    {
    }
};

struct Com2 : public IRRPSCallbackInit
{
private:
    std::string id{"com2"};
    RandInt randInt{0, 3};
    rrps::ResponseCallback<ArgumentType, ArgumentType> com1_get_arg;

public:
    void operator()()
    {
        std::this_thread::sleep_for(std::chrono::seconds(randInt()));
        if(com1_get_arg) {
            auto a = com1_get_arg(ArgumentType());
            std::cout << "Arg requested: a:" << a.a << ", b: " << a.d << std::endl;
        }
    }

    void submit_publish_and_response_sercices(std::shared_ptr<rrps::IServiceProvider<ArgumentType, ArgumentType>> rrpsMGR)
    {
    }

    void submit_subscribe_and_requests_services(std::shared_ptr<rrps::IServiceRequester<ArgumentType, ArgumentType>> rrpsMGR)
    {
        rrpsMGR->register_subscriber(id, "com1_arg_updated", [](ArgumentType arg) { std::cout << "Arg updated: a:" << arg.a << ", b: " << arg.d << std::endl; } );

        com1_get_arg = rrpsMGR->get_response_service(id, "com1_get_arg");
    }
};