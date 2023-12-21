cmake_minimum_required(VERSION 3.27)
project(BITAutoLogin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS -static)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(BITAutoLogin
        main.cpp
        xencode.cpp
        xencode.h
        decode.cpp
        decode.h
        requests.cpp
        main.h
        generate.cpp
        generate.h
        requests.h
        WindowsService.cpp)
target_link_libraries(BITAutoLogin PRIVATE CURL::libcurl)
target_link_libraries(BITAutoLogin PRIVATE OpenSSL::SSL OpenSSL::Crypto)

