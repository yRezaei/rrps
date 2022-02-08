#ifndef C9D537A4_A1DD_4773_AC3E_6B72A2EB5D8B
#define C9D537A4_A1DD_4773_AC3E_6B72A2EB5D8B

#include <memory>
#include <basic_types.hpp>

template <class T_return, class T_arg>
using ProvideServiceRegistratorSPtr = std::shared_ptr<rrps::IProvideServiceRegistrator<T_return, T_arg>>;

template <class T_return, class T_arg>
using ReceiveServiceRegistratorSPtr = std::shared_ptr<rrps::IReceiveServiceRegistrator<T_return, T_arg>>;

/*
    mtcs: Multi-threaded component system
*/
namespace mtcs
{
    template <typename T_return, typename T_arg>
    class Component
    {
    public:
        template <typename ComponentType>
        Component(const ComponentType &value) = delete;

        template <typename ComponentType>
        Component(ComponentType &&value) : value_(std::make_unique<Model<ComponentType, T_return, T_arg>>(value)) {}

        template <typename ComponentType>
        Component &operator=(ComponentType &&value)
        {
            value_.reset(new Model<ComponentType, T_return, T_arg>(value));
            return *this;
        }

        template <typename ComponentType>
        Component &operator=(const ComponentType &value) = delete;

        void execute()
        {
            value_->execute();
        }

        void submit_provide_services(ProvideServiceRegistratorSPtr<T_return, T_arg> provider_registrator)
        {
            value_->submit_provide_services(provider_registrator);
        }

        void submit_receive_services(ReceiveServiceRegistratorSPtr<T_return, T_arg> request_registrator)
        {
            value_->submit_receive_services(request_registrator);
        }

    private:
        template <typename T_return_concept, typename T_arg_concept>
        struct Concept
        {
            virtual ~Concept() = default;
            virtual void execute() = 0;
            virtual void submit_provide_services(ProvideServiceRegistratorSPtr<T_return_concept, T_arg_concept> provider_registrator) = 0;
            virtual void submit_receive_services(ReceiveServiceRegistratorSPtr<T_return_concept, T_arg_concept> request_registrator) = 0;
        };

        template <typename ComponentType, typename T_return_model, typename T_arg_model>
        struct Model : Concept<T_return_model, T_arg_model>
        {
            Model(ComponentType &&value) : val(value) {}
            void execute() override
            {
                val.execute();
            }

            void submit_provide_services(ProvideServiceRegistratorSPtr<T_return_model, T_arg_model> provider_registrator)
            {
                val.submit_provide_services(provider_registrator);
            }

            void submit_receive_services(ReceiveServiceRegistratorSPtr<T_return_model, T_arg_model> request_registrator)
            {
                val.submit_receive_services(request_registrator);
            }

            ComponentType val;
        };

    private:
        std::unique_ptr<Concept<T_return, T_arg>> value_;
    };
}

#endif /* C9D537A4_A1DD_4773_AC3E_6B72A2EB5D8B */
