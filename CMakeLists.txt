cmake_minimum_required(VERSION 3.13)  # CMake version check
project(crypto)               # Create project "crypto"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES main.cpp AES.cpp AES.h helper.cpp helper.h)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(crypto ${SOURCE_FILES})