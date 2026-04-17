cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


project(TRDP-PROTOCOLE)


add_executable(setup_protocole main.cpp)



target_include_directories(setup_protocole PRIVATE
    /usr/local/include/trdp
)

target_link_libraries(setup_protocole PRIVATE
    trdp
    trdpap
    pthread
    uuid
)

set(CMAKE_BUILD_TYPE Debug)
