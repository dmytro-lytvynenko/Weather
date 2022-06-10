// application.cpp
#include "application.hpp"
#include "client.hpp"

Application::Application(int argc, char** argv) : m_argc(argc), m_argv(argv), desc("Options")
{ }

int Application::exec()
{
  std::string defaultPort = "80";
  std::string defaultAddress = "api.openweathermap.org";

  desc.add_options()
    ("help,h", "Show help")
    ("city,c", po::value<std::string>(), 
                            "Enter city (If the city name is divided into " 
                            "several words - use _ instead of space)")
    ("token,t", po::value<std::string>(), 
                            "Enter token")
    ("address,a", po::value<std::string>(), 
                            "Enter host address")
    ("port,p", po::value<std::string>(), 
                            "Enter port");

  po::variables_map vm;
  
  try
  {
      po::store(po::parse_command_line(m_argc, m_argv, desc), vm);
  }
  catch(...)
  {
      std::cout << "Error while parsing command line" << std::endl;
      return 0;
  }

  if(vm.count("help") || vm.empty())
      std::cout << desc << std::endl;

  if(vm.count("address"))
      defaultAddress = vm["address"].as<std::string>();
  
  if(vm.count("port"))
      defaultPort = vm["port"].as<std::string>();

  ClientLayer::Client client(std::make_shared<boost::asio::io_context>(), defaultAddress, defaultPort);

  if(vm.count("city"))
      client.setCity(vm["city"].as<std::string>());

  if(vm.count("token"))
      client.setToken(vm["token"].as<std::string>());
  
  try
  {
      client.process();
  }
  catch(...)
  {
      std::cout << "Can't get weather in " << client.getCity() << std::endl;
      return 1;
  }

  return 0;
}
