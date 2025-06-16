#pragma once
#include "parameter.hpp"
#include "request.hpp"
#include "response.hpp"
#include "connection.hpp"
#include <boost/asio.hpp>
#include <future>

namespace http_client::runner {
    struct client_runner {
        boost::asio::any_io_executor _executor;

        explicit client_runner(boost::asio::io_context &io_context) : _executor(io_context.get_executor()) {
        }

        void operator()() {
            http_client::parameter::client_parameter params;
            params._executor = _executor;
            const std::shared_ptr<http_client::client::_http_client> client = http_client::client::_http_client::make(
                std::move(params));


            http_client::connection::client_connection connection;
            connection.host = "httpbin.org";
            connection.port = "80";

            std::string message =
                    "GET /get HTTP/1.1" "\r\n"
                    "Host: httpbin.org" "\r\n"
                    "\r\n"
                    ;

            http_client::request::http_request request;
            std::promise<std::pair<std::error_code, http_client::response::http_response> > operation_promise;
            auto operation_future = operation_promise.get_future();

            client->connect_async(std::move(connection), message, [
                                      &operation_promise
                                  ](
                              std::error_code err,
                              http_client::response::http_response response
                          ) {
                                      operation_promise.set_value(std::make_pair(err, std::move(response)));
                                  });

            auto [error, response] = operation_future.get();

            if (error) {
                std::cerr << "Operation failed: " << error.message() << std::endl;
                return;
            }

            std::cout << "Response Data" << response.data << std::endl;
        }
    };
}
