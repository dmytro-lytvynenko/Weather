// networklayer.cpp
#include "network.hpp"
using namespace NetworkLayer;

Network::Network(const ContextPtr & context_ptr, const IPAddress & connectionIP) 
               : io_context(context_ptr), resolver(*context_ptr),
                 m_stream(*context_ptr), m_connectionIP(std::move(connectionIP))
{ }

bool Network::start()
{
    auto res = resolver.resolve(m_connectionIP.host, m_connectionIP.port, error_code);

    if (error_code) 
    {
        std::cout << "Resolver error" << std::endl;
        return false;
    }

    m_stream.connect(res, error_code);

    if (error_code) 
    {
        std::cout << "Can't reach server" << std::endl;
        return false;
    }

    return m_stream.socket().is_open();
}

void Network::send(const std::string & city_name, const std::string & token) 
{
    http::request<http::string_body> req{http::verb::get, 
    "/data/2.5/weather?q=" + city_name + "&appid=" + token + "&units=metric", 11};
    
    req.set(http::field::host, m_connectionIP.host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    send(req);
}

void Network::send(const http::request<http::string_body> & request)
{
    if (!error_code && m_stream.socket().is_open())
    {
        http::write(m_stream, request, error_code);
        if(error_code)
            std::cout << "Can't write request" << std::endl;
    }
}

std::string Network::receive()
{
    return boost::beast::buffers_to_string(p_receive().body().data());
}

http::response<http::dynamic_body> Network::p_receive()
{
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;

    if(!error_code)
    {
        http::read(m_stream, buffer, res, error_code);
        if(error_code)
            std::cout << "Can't read response" << std::endl;
    }

    return res;
}

Network::~Network()
{ 
    if(error_code)
        std::cout << "Error code: " << error_code << std::endl;

    if(!error_code && m_stream.socket().is_open())
        m_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}