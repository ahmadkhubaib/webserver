#pragma once
#include <boost/asio.hpp>
#include <utility>
#include "stream_populator.hpp"

namespace http_client::parser {
    struct stream_parser {
        explicit stream_parser(const http_client::parser::stream_populator *populator)
            : _populator(populator) {
        }

        http_client::parser::stream_populator _populator;

        typedef boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> iterator;

        template<typename iterator>
        std::pair<iterator, bool> operator()(iterator begin_iterator, iterator end_iterator) {
            return (_populator)(begin_iterator, end_iterator);
        }
    };
};

namespace boost {
    namespace asio {
        template<>
        struct is_match_condition<http_client::parser::stream_parser> : public boost::true_type {
        };
    }
}
