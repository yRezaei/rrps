#include <iostream>
#include <rrps.hpp>
#include <component.hpp>
#include <memory>
#include <thread>
#include <chrono>
#include <random>

struct BaseObj
{
    int a;
    double d;
};

struct Com1
{
private:
    const std::string id{"com1"};
    BaseObj arg{1, 5.5};
    std::unordered_map<std::string, rrps::SubCallback<BaseObj>> sub_callback_list;
    std::mutex mtx;

public:
    void execute()
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            arg.a++;
            arg.d++;
        }

        for (auto &sub : sub_callback_list)
        {
            sub.second(arg);
        }
    }

    void submit_provide_services(std::shared_ptr<rrps::IProvideServiceRegistrator<BaseObj, BaseObj>> provide_registrator)
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
                                                                      { std::lock_guard<std::mutex> lock(mtx); return arg; }; });
    }

    void submit_receive_services(std::shared_ptr<rrps::IReceiveServiceRegistrator<BaseObj, BaseObj>> request_registrator)
    {
    }
};

struct Com2
{
private:
    const std::string id{"com2"};
    mutable rrps::ResponseCallback<BaseObj, BaseObj> com1_get_arg;

public:
    void execute()
    {
        if (com1_get_arg)
        {
            auto a = com1_get_arg(BaseObj());
            std::cout << "Arg requested: a:" << a.a << ", b: " << a.d << std::endl;
        }
    }

    void submit_provide_services(std::shared_ptr<rrps::IProvideServiceRegistrator<BaseObj, BaseObj>> provide_registrator)
    {
    }

    void submit_receive_services(std::shared_ptr<rrps::IReceiveServiceRegistrator<BaseObj, BaseObj>> request_registrator)
    {
        request_registrator->register_subscriber(id, "com1_arg_updated", [](BaseObj arg)
                                                 { std::cout << "Arg updated: a:" << arg.a << ", b: " << arg.d << std::endl; });

        com1_get_arg = request_registrator->get_response_service(id, "com1_get_arg");
    }
};

using Components = std::vector<mtcs::Component<BaseObj, BaseObj>>;

int main(int argc, char const *argv[])
{
    auto rrps_service_mgr = std::make_shared<rrps::ReqResPubSubManager<BaseObj, BaseObj>>();
    Components components{};
    Com1 c1;
    Com2 c2;
    components.emplace_back(c1);
    components.emplace_back(c2);

    try
    {
        for (auto& comp : components)
        {
            comp.submit_provide_services(rrps_service_mgr);
        }

        for (auto& comp : components)
        {
            comp.submit_receive_services(rrps_service_mgr);
        }

        for (auto i = 0u; i < 10; ++i)
        {
            for (auto& comp : components)
            {
                comp.execute();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}