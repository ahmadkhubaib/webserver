#pragma once
#include <memory>

#include "parameter.hpp"
#include "request.hpp"

namespace http_client::client {
    struct _http_client
    {
        static std::shared_ptr<_http_client> make(parameter::client_parameter parameter)
        {
            return std::make_shared<_http_client>();
        }

        template<typename T>
        void send_async(const request::http_request & request, T func){}
    };
}
