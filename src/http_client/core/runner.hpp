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
            auto client = http_client::client::_http_client::make(std::move(params));


            http_client::connection::client_connection connection;
            connection.host = "google.com";
            connection.port = "80";

            std::promise<std::error_code> connect_promise;
            auto connect_future = connect_promise.get_future();

            client->connect_async(std::move(connection), [&connect_promise](std::error_code err) {

                if (err) {
                    std::cerr << "Connection error: " << err.message() << std::endl;
                } else {
                    std::cout << "Connected successfully!" << std::endl;
                }
                connect_promise.set_value(err);
            });

            if (std::error_code connect_result = connect_future.get()) {
                std::cerr << "Failed to connect: " << connect_result.message() << std::endl;
                return;
            }

            http_client::request::http_request request;
            std::promise<std::error_code> send_promise;
            auto send_future = send_promise.get_future();
            client
                    ->
                    send_async(request, [&send_promise](std::error_code err,
                                                        http_client::response::http_response response) {
                                   if (err) {
                                       std::cerr << "Error: " << err.message() << std::endl;
                                       send_promise.set_value(err);
                                   } else {
                                       std::cout << "Response body: " << response.body << std::endl;
                                       std::cout << "Response status code: " << response.status_code << std::endl;
                                       send_promise.set_value(std::error_code());
                                   }
                               }

                    );
            std::error_code send_result = send_future.get();
            if (send_result) {
                std::cerr << "Failed to send request: " << send_result.message() << std::endl;
            }
        }
    };
}
