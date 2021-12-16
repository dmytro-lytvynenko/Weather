// networklayer.cpp
#include "network.hpp"
using namespace NetworkLayer;

Network::Network(const ContextPtr & context_ptr, const IPAddress & connectionIP, const std::string & city_name) 
               : io_context(context_ptr), resolver(*context_ptr), m_socket(*context_ptr), m_city(city_name)
{ 
    this->connectionIP.host = connectionIP.host;
    this->connectionIP.port = connectionIP.port;
}

bool Network::start()
{
    boost::asio::connect(m_socket, resolver.resolve(connectionIP.host, connectionIP.port, error_code), 
    error_code);

    return (m_socket.is_open() && error_code);
}

void Network::send(const std::string & city_name, const std::string & token)
{
    http::request<http::string_body> req(http::verb::get, 
    "api.openweathermap.org/data/2.5/weather?q=" + city_name + "&appid=" + token, 11);
    
    req.set(http::field::host, connectionIP.host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    send(req);
}

void Network::send(const http::request<http::string_body> & request)
{
    if (m_socket.is_open() && !error_code)
        http::write(m_socket, request);
}

std::string Network::receive()
{
     try
     {
        std::stringstream jsonEncod(beast::buffers_to_string(p_receive().body().data()));
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncod, root);
        try
        {
            if (!root.get<std::string>("message").empty())
                return "Can't get weather in " + m_city + "\n";
        }
        catch (...)
        { }

        return "City: " + root.get<std::string>("name") + "\n" +
               "Temperature: " + root.get<std::string>("main.temp") + "°C\n" +
               "Wind velocity: " + root.get<std::string>("wind.speed") + " meter/sec\n" +
               "Wind direction: " + root.get<std::string>("wind.deg") + "°\n";
    }
    catch(std::exception &e)
    {
        return "Error\n";
    }
}

http::response<http::dynamic_body> Network::p_receive()
{
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;

    if(m_socket.is_open() && !error_code)
        http::read(m_socket, buffer, res);

    return res;
}

Network::~Network()
{
    if(m_socket.is_open() && !error_code)
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}