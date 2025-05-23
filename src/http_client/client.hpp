#pragma once
#include <memory>
#include "http_client/parameter.hpp"

namespace http_client
{
    namespace client
    {
        struct http_client
        {
            static std::shared_ptr<http_client> make(parameter::client_parameter &param)
            {
                return std::make_shared<http_client>();
            }
        };
    }
}