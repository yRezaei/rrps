#ifndef RRPS_BASICE_TYPES_HPP_
#define RRPS_BASICE_TYPES_HPP_

#include <string>
#include <functional>

namespace rrps
{
    template<class T_arg>
    using SubCallback = std::function<void(T_arg)>;
    
    template<class T_arg>
    using SubscribeServiceRegisterCallback = std::function<void(const std::string&, SubCallback<T_arg>)>;
    
    template<class T_return, class T_arg>
    using ResponseCallback = std::function<T_return(T_arg)> ;
    
    template<class T_return, class T_arg>
    using ResponseServiceRegisterCallback = std::function<ResponseCallback<T_return, T_arg>(const std::string&)>;
}

#endif // end of RRPS_BASICE_TYPES_HPP_