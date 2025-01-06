#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE                     // Use only ASIO library (No Boost)
#include <asio/registered_buffer.hpp>
#include <system_error>
#include <thread>
#endif

#include <iostream>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(1 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if(!ec)
            {
                std::cout << "\n\nRead " << length << "bytes\n\n";
                for (int i = 0; i < length; i++)
                    std::cout << vBuffer[i];

                GrabSomeData(socket);
            }
        });
}

int main()
{
    std::cout << "Hello World!" << std::endl;

    asio::error_code ec;        // For error handling
    asio::io_context context;   // Main asio object

    // Give some fake tasks to asio so the context doesnt finish
    asio::io_context::work idleWork(context);

    // Start the context
    std::thread thrContext = std::thread([&]() { context.run(); });

    // connect to example.com
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

    // Create a socket
    asio::ip::tcp::socket socket(context);

    // Try and connect to the given address
    socket.connect(endpoint, ec);

    if(!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cerr << "Failed to connect to address:\n[ERROR]:" << ec.message() << std::endl;
    }

    if(socket.is_open())
    {
        GrabSomeData(socket);

        // HTTP Request
        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        // Send request
        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        // do something else, while asio handles data transfer in background
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20000ms);

        context.stop();
        if (thrContext.joinable()) thrContext.join();
    }

    return 0;
}
