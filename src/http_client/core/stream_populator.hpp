#pragma once
#include <boost/asio.hpp>
#include <utility>

namespace http_client::parser {
    struct stream_populator {
        typedef boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> iterator;

        const bool done = true;
        const bool not_done = false;

        template<typename iterator>
        std::pair<iterator, bool> operator()(iterator begin_iterator, iterator end_iterator) {
            if (auto size = std::distance(begin_iterator, end_iterator) > 400 ) {
                return std::make_pair(end_iterator, done);
            } else {
                return std::make_pair(begin_iterator, not_done);
            }

        }
    };
}
