cmake_minimum_required(VERSION 3.0)
project(learn-memory-model)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pthread -O3")

add_executable(${PROJECT_NAME} main.cpp)

find_package(Boost REQUIRED COMPONENTS thread program_options REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
        )