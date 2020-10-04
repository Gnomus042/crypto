cmake_minimum_required(VERSION 3.13)  # CMake version check
project(crypto)               # Create project "crypto"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
ADD_DEFINITIONS("-std=gnu++0x")
# Add main.cpp file of project root directory as source file
set(
        SOURCE_FILES
        main.cpp
        AES.cpp
        AES.h
        helper.cpp
        helper.h
        Kalyna.cpp
        Kalyna.h
)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(crypto ${SOURCE_FILES})

# Add googletest
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


set(
        TESTS
        test/test.cpp
)

add_executable(crypto_unittest ${TESTS} ${SOURCE_FILES})

target_link_libraries(crypto_unittest PUBLIC gtest_main)
