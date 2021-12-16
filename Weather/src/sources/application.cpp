// application.cpp
#include "application.hpp"
#include "client.hpp"

Application::Application(int argc, char** argv) : m_argc(argc), m_argv(argv), desc("Options")
{ }

int Application::exec()
{
  std::string task_type;

  desc.add_options()
    ("help,h", "Show help")
    ("city,c", po::value<std::string>(&task_type), 
                            "Enter city (If the city name is divided into several\r\n" 
                            "words - use _ instead of space)")
    ("token,t", po::value<std::string>(&task_type), 
                            "Enter token")
    ("address,a", po::value<std::string>(&task_type), 
                            "Enter host address")
    ("port,p", po::value<std::string>(&task_type), 
                            "Enter port")
    ;

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(m_argc, m_argv).options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);
  
  ClientLayer::Client client(std::make_shared<boost::asio::io_context>(), "api.openweathermap.org", "80");

  if(vm.count("help"))
      std::cout << desc << std::endl;

  if(vm.count("city"))
      client.setCity(vm["city"].as<std::string>());

  if(vm.count("token"))
      client.setToken(vm["token"].as<std::string>());

  if(vm.count("address"))
      client.setAddress(vm["address"].as<std::string>());
  
  if(vm.count("port"))
      client.setAddress(vm["port"].as<std::string>());

  client.process();

  return 0;
}

Application::~Application()
{
    delete[] m_argv;
}