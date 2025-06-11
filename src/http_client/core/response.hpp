#pragma once

#include <string>

namespace http_client::response {
    struct http_response
    {
        std::string body;
        int status_code = 400;
    };
}
