#pragma once
#include <memory>
#include "http_client/parameter.hpp"
#include "http_client/common/request.hpp"
#include "http_client/common/response.hpp"

namespace http_client
{
    namespace client
    {
        struct _http_client
        {
            static std::shared_ptr<_http_client> make(http_client::parameter::client_parameter parameter)
            {
                return std::make_shared<_http_client>();
            }

            template<typename T>
            void send_async(const request::http_request & request, T func){}
        };
    }
}