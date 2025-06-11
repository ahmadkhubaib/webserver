#pragma once

namespace http_client::connection {
    struct client_connection {
        std::string host;
        std::string port;
    };
}