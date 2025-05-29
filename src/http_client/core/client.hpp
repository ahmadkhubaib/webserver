#pragma once
#include <memory>

#include "parameter.hpp"
#include "request.hpp"
#include "response.hpp"

namespace http_client::client {
    struct _http_client
    {
        static std::shared_ptr<_http_client> make(parameter::client_parameter parameter)
        {
            return std::shared_ptr<_http_client>(new _http_client(std::move(parameter)));
        }

        template<typename T>
        void send_async(const request::http_request & request, T func) {
            boost::asio::post(_parameter._executor, [f = std::move(func)]() {
                response::http_response response{};
                response.body = "Hello from client.hpp line 20";
                f(std::error_code(), std::move(response));
            });
        }

    private:
        parameter::client_parameter _parameter;
        _http_client(parameter::client_parameter parameter) : _parameter(std::move(parameter)) {}
    };
};
