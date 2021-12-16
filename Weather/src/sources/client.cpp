// client.cpp
#include "client.hpp"
using namespace ClientLayer;

Client::Client(const NetworkLayer::ContextPtr & context_ptr, 
               const std::string host, const std::string port)
                : m_connection(new NetworkLayer::Network (context_ptr, {host,port}, m_city))
{ }

void Client::setCity(const std::string & city_name)
{
    m_city = city_name;   
}

void Client::setToken(const std::string & token)
{
    m_token = token;
}

void Client::setAddress(const std::string & address)
{
    m_address = address;
}

void Client::setPort(const std::string & port)
{
    m_port = port;
}

bool Client::getToken()
{
    std::ifstream token("token.txt");
    if (!token)
        return 0;

    else 
    {
        token >> m_token;
        return 1;
    }
}

void Client::process()
{
  if (m_token.empty() && !getToken())
  {
    std::cout << "Can't find token or token.txt file";
    return;
  }

  if(m_connection->start())
  {
    m_connection->send(m_city, m_token);
    std::cout << m_connection->receive();
  }
  else
  {
    std::cout << "Can't reach server" << std::endl;
    return;
  }
}