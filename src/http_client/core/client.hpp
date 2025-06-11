#pragma once
#include <memory>

#include "parameter.hpp"
#include "request.hpp"
#include "response.hpp"
#include "connection.hpp"

namespace http_client::client {
    struct _http_client : std::enable_shared_from_this<http_client::client::_http_client> {
        static std::shared_ptr<_http_client> make(http_client::parameter::client_parameter parameter) {
            return std::shared_ptr<_http_client>(new _http_client(std::move(parameter)));
        }

        template<typename T>
        void send_async(const request::http_request &request, T func) {
            boost::asio::post(_parameter._executor, [callback = std::move(func)]()mutable {
                http_client::response::http_response response{};
                response.body = "Hello from client.hpp line 19";
                callback(std::error_code(), std::move(response));
            });
        }

        template<typename T>
        void connect_async(
            const http_client::connection::client_connection connection,
            const request::http_request &request,
            T func
        ) {
            _resolver.async_resolve(
                connection.host,
                connection.port,
                [
                    localThis = this->shared_from_this(),
                    callback = std::move(func),
                    request
                ](
            const boost::system::error_code ec,
            const boost::asio::ip::tcp::resolver::results_type &results
        ) mutable {
                    if (ec) {
                        std::cout << "Error resolving: " << ec.message() << "\n";
                        return callback(ec, http_client::response::http_response{});
                    }
                    boost::asio::async_connect(
                        this->_socket,
                        results,
                        [
                            callback_ac = std::move(callback),
                            localThis = std::move(localThis),
                            request
                        ](
                    const boost::system::error_code ec,
                    const boost::asio::ip::tcp::endpoint endpoint
                ) {
                            if (ec) {
                                std::cout << "Error : " << ec.message() << "\n";
                                return callback_ac(ec, http_client::response::http_response{});
                            }
                            std::cout << "Connected successfully to " << endpoint << "\n";

                            localThis->send_async(request, std::move(callback_ac));
                        });
                });
        }

    private:
        http_client::parameter::client_parameter _parameter;
        boost::asio::ip::tcp::resolver _resolver;
        boost::asio::ip::tcp::socket _socket;

        explicit _http_client(http_client::parameter::client_parameter parameter)
            : _parameter(std::move(parameter)),
              _resolver(_parameter._executor),
              _socket(_parameter._executor) {
        }
    };
};
