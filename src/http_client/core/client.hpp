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
        void send_async(const std::string &message, T func) {
            // Create request
            std::ostream os(&_request_buffer);
            os << message;


            boost::asio::async_write(_socket, _request_buffer,
                                     [
                                         this,
                                         local_this = this->shared_from_this(),
                                         func = std::move(func)
                                     ](
                                 boost::system::error_code ec, std::size_t bytes_written) {
                                         _request_buffer.consume(bytes_written);

                                         if (ec) {
                                             func(ec, http_client::response::http_response{});
                                             return;
                                         }
                                         read_response_async(std::move(func));
                                     });
        }

        template<typename T>
        void connect_async(
            const http_client::connection::client_connection connection,
            std::string &message,
            T func
        ) {
            _resolver.async_resolve(
                connection.host,
                connection.port,
                [
                    local_this = this->shared_from_this(),
                    callback = std::move(func),
                    message,
                    this
                ](
            const boost::system::error_code &ec,
            const boost::asio::ip::tcp::resolver::results_type &results
        ) mutable {
                    if (ec) {
                        std::cout << "Error resolving: " << ec.message() << "\n";
                        prevent_deletion();
                        callback(ec, http_client::response::http_response{});
                        return;
                    }
                    boost::asio::async_connect(
                        local_this->_socket,
                        results,
                        [
                            callback_ac = std::move(callback),
                            local_this = std::move(local_this),
                            message,
                            this
                        ](
                    const boost::system::error_code &ec,
                    const boost::asio::ip::tcp::endpoint &endpoint
                ) {
                            if (ec) {
                                std::cout << "Error : " << ec.message() << "\n";
                                prevent_deletion();
                                return callback_ac(ec, http_client::response::http_response{});
                            }
                            std::cout << "Connected successfully to " << endpoint << "\n";

                            prevent_deletion();
                            local_this->send_async(message, callback_ac);
                        });
                });
        }

    private:
        http_client::parameter::client_parameter _parameter;
        boost::asio::ip::tcp::resolver _resolver;
        boost::asio::ip::tcp::socket _socket;
        boost::asio::streambuf _request_buffer;
        boost::asio::streambuf _response_buffer;

        explicit _http_client(http_client::parameter::client_parameter parameter)
            : _parameter(std::move(parameter)),
              _resolver(_parameter._executor),
              _socket(_parameter._executor) {
        }

        template<typename T>
        void read_response_async(T callable) {
            boost::asio::async_read_until(_socket, _response_buffer, "\r\n\r\n",
                                          [this, local_this = this->shared_from_this(), callable = std::move(callable)](
                                      boost::system::error_code ec, size_t bytes_read) mutable {
                                              if (ec) {
                                                  std::cout << "Error reading response: " << ec.message() << "\n";
                                                  callable(ec, http_client::response::http_response{});
                                                  return;
                                              }

                                              std::string data{
                                                  std::istreambuf_iterator(&_response_buffer),
                                                  std::istreambuf_iterator<char>()
                                              };
                                              http_client::response::http_response response;
                                              response.data = std::move(data);
                                              callable(std::error_code(), std::move(response));
                                          });
        }

        void send_message_async(boost::asio::ip::tcp::socket _socket, const std::string &message) {
            std::ostream os(&_request_buffer);
            os << message;

            boost::asio::async_write(_socket, _request_buffer,
                                     [local_this = this->shared_from_this()
                                     ](boost::system::error_code ec, std::size_t) {
                                         //TODO
                                     });
        }

        void prevent_deletion() {
            static std::vector<std::shared_ptr<_http_client> > keep_alive;
            keep_alive.push_back(shared_from_this());

            if (keep_alive.size() > 100) {
                keep_alive.erase(keep_alive.begin());
            }
        }
    };
};
