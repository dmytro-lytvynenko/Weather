// client.cpp
#include "client.hpp"
using namespace ClientLayer;

Client::Client(const NetworkLayer::ContextPtr & context_ptr, 
               const std::string & host, const std::string & port)
               : m_connection(new NetworkLayer::Network (context_ptr, {std::move(host), std::move(port)}))
{ 
    std::fstream token("token.txt");
    //token.open("../token.txt");
    if(token.is_open()) 
        token >> m_token;
}

void Client::setCity(const std::string & city_name)
{
    m_city = city_name;   
}

void Client::setToken(const std::string & token)
{
    m_token = token;
}

void Client::process()
{
  if (!m_city.empty())
  {
        if (m_token.empty())
        {
            std::cout << "Can't find token or token.txt file" << std::endl;
            //std::cout << m_token;
            return;
        }

        if (!m_connection->start())
        {
            std::cout << "Can't reach server" << std::endl;
            return;
        }
    
    m_connection->send(m_city, m_token);
    std::cout << parse(m_connection->receive());
  }
}

std::string Client::parse(const std::string & json)
{
    //std::cout << json << std::endl;
    std::stringstream jsonEncod(json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(jsonEncod, root);

    try
    {
        std::string result = "City: " + root.get<std::string>("name") + "\n" +
                             "Temperature: " + root.get<std::string>("main.temp") + "°C\n" +
                             "Wind's speed: " + root.get<std::string>("wind.speed") + " meter/sec \n" +
                             "Wind's direction: " + root.get<std::string>("wind.deg") + "°\n";
        return result;

    }
    catch (...)
    {
        /*std::cout << m_token << std::endl;
        std::cout << json << std::endl;
        std::cout << std::string(e.what()) << std::endl;*/
        return "Can't get weather in " + m_city + "\n";
    }

}

std::string Client::getCity() const
{
    return m_city;
}