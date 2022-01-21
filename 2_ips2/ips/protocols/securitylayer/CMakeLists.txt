cmake_minimum_required(VERSION 3.5)

project(enc_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories ("${PROJECT_SOURCE_DIR}/ipscrypt")

#add_subdirectory(ipscrypt/AES)
#add_subdirectory(ipscrypt/MD5)
add_subdirectory(ipscrypt)

add_executable(enc_test test.cpp)
target_link_libraries(${PROJECT_NAME} ips_aes)
target_link_libraries(${PROJECT_NAME} ips_md5)
target_link_libraries(${PROJECT_NAME} ipscrypt)
