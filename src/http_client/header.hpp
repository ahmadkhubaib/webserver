#pragma once
#include <boost/asio.hpp>

namespace http_client
{
    namespace header
    {
        struct client_header
        {
            boost::asio::any_io_executor _executor;

            explicit client_header(boost::asio::io_context &io_context) : _executor(io_context.get_executor()) {}

            void operator()()
            {

            }
        };
    }
}