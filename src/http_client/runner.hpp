#pragma once
#include "http_client/parameter.hpp"
namespace http_client
{
    namespace runner
    {
        struct client_runner
        {
            boost::asio::any_io_executor _executor;

            explicit client_runner(boost::asio::io_context &io_context) : _executor(io_context.get_executor()) {}

            void operator()()
            {
                http_client::parameter::client_parameter params;
                params._executor = _executor;
                auto client = http_client::client::_http_client::make(std::move(params));

            }
        };
    }
}