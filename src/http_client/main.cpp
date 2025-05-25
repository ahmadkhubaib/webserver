#include <iostream>
#include <boost/asio.hpp>
#include "core.hpp"

int main()
{

    boost::asio::io_context io_context;

    auto dummyWork = std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
        boost::asio::make_work_guard(io_context)
    );

    http_client::runner::client_runner runner(io_context);
    runner();


    std::thread t([&io_context](){
        try{
            io_context.run();
        }
        catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << "\n";
        } });
    std::cout << "Hello, World! from Client" << std::endl;
    return 0;
}