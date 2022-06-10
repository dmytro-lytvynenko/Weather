#pragma once
#include <fstream>
#include "network.hpp"

namespace ClientLayer
{
    using tcp = NetworkLayer::tcp;

    class Client
    {
    public:
        /**
         * @brief Ctor
         * @param context_ptr - smart pointer to io_context
         * @param host - server address
         * @param port - server port
         * */
        Client(const NetworkLayer::ContextPtr & context_ptr, 
               const std::string & host, const std::string & port);

        /**
         * @brief Main function that connections to server, sends request and shows result
         * */
        void process();

        void setCity(const std::string & city_name);
        void setToken(const std::string & token);
        std::string getCity() const;

    private:
        
        std::string parse(const std::string & json);

        std::shared_ptr<NetworkLayer::Network> m_connection;

        std::string m_city;
        std::string m_token;
    };
}