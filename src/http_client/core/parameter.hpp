#pragma once
namespace http_client
{
    namespace parameter
    {
        struct client_parameter
        {
            boost::asio::any_io_executor _executor;
        };
    }
}