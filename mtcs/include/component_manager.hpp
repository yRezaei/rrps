#ifndef D4C60535_994A_4205_A05D_81F0E5888FFF
#define D4C60535_994A_4205_A05D_81F0E5888FFF

#include "component.hpp"
#include <provider_recipient_service.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

namespace mtcs
{
    template <typename T_return, typename T_arg>
    class ComponentManager
    {
    private:
        std::unordered_map<std::string, std::vector<Component<T_return, T_arg>>> component_groups;
        std::shared_ptr< prs::ProviderRecipientService<T_return, T_arg>> prs_mgr{new prs::ProviderRecipientService<T_return, T_arg>()};

    public:
        ComponentManager(/* args */) = default;
        ~ComponentManager() = default;

        template<typename CompType>
        ComponentManager &add_component(const std::string &component_group, CompType comp)
        {
            component_groups[component_group].emplace_back(comp);
            return *this;
        }

        void init()
        {
            for (auto &comp_group : component_groups)
            {
                for (auto &comp : comp_group.second)
                {
                    comp.submit_provide_services(prs_mgr);
                }
            }

            for (auto &comp_group : component_groups)
            {
                for (auto &comp : comp_group.second)
                {
                    comp.submit_receive_services(prs_mgr);
                }
            }
        }

        void run()
        {
            for (auto &comp_group : component_groups)
            {
                for (auto &comp : comp_group.second)
                {
                    comp.execute();
                }
            }
        }
    };
}

#endif /* D4C60535_994A_4205_A05D_81F0E5888FFF */
