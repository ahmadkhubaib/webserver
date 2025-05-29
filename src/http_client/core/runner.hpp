#pragma once
#include "parameter.hpp"
#include "request.hpp"
#include "response.hpp"

namespace http_client::runner {
    struct client_runner {
#include <boost/asio.hpp>
        boost::asio::any_io_executor _executor;

        explicit client_runner(boost::asio::io_context &io_context) : _executor(io_context.get_executor()) {
        }

        void operator()() {
            parameter::client_parameter params;
            params._executor = _executor;
            auto client = client::_http_client::make(std::move(params));

            request::http_request request;
            client->send_async(request, [](std::error_code err, response::http_response response) {
                if (err) {
                    std::cerr << "Error: " << err.message() << std::endl;
                } else {
                    std::cout << "Response body: " << response.body << std::endl;
                    std::cout << "Response status code: " << response.status_code << std::endl;
                }
            });
        }
    };
}
