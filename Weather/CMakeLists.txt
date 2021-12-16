cmake_minimum_required(VERSION 3.14)

set(This Weather)

project(${This} CXX)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)

file(GLOB Sources "src/sources/*.cpp")
file(GLOB Headers "src/headers/*.hpp")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(${This} ${Sources} ${Headers})

target_include_directories(${This} PUBLIC "src/headers")
target_link_libraries(${This} PUBLIC Threads::Threads Boost::program_options Boost::system Boost::filesystem)