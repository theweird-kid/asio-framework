#include <experimental/internet>
#include <experimental/net>

namespace net = std::experimental::net;

int main(int argc, char* argv[])
{
    net::ip::tcp::iostream s("www.boost.org", "http");

}
