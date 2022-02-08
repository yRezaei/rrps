#ifndef AA3A8B5D_9A0B_4C1E_B8C4_D4A5087348D1
#define AA3A8B5D_9A0B_4C1E_B8C4_D4A5087348D1

#include <string>
#include <functional>

/*
    prs: Request-Respons & Publisher-Subscriber
*/
namespace prs
{
    template <class T_arg>
    using SubCallback = std::function<void(T_arg)>;

    template <class T_arg>
    using SubscribeServiceRegisterCallback = std::function<void(const std::string &, SubCallback<T_arg>)>;

    template <class T_return, class T_arg>
    using ResponseCallback = std::function<T_return(T_arg)>;

    template <class T_return, class T_arg>
    using ResponseServiceRegisterCallback = std::function<ResponseCallback<T_return, T_arg>(const std::string &)>;
}

#endif /* AA3A8B5D_9A0B_4C1E_B8C4_D4A5087348D1 */
