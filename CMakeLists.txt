cmake_minimum_required(VERSION 3.13)  # CMake version check
project(crypto)               # Create project "crypto"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
ADD_DEFINITIONS("-std=gnu++0x")
# Add main.cpp file of project root directory as source file
set(
        SOURCE_FILES
        Lab1/AES.cpp
        Lab1/AES.h
        helper.cpp
        helper.h
        kalyna_tables.cpp
        kalyna_tables.h
        Lab1/Kalyna.cpp
        Lab1/Kalyna.h
        Lab2/RC4.cpp
        Lab2/RC4.h
        Lab2/Salsa20.cpp
        Lab2/Salsa20.h
        Lab3/SHA256.cpp
        Lab3/SHA256.h
        Lab3/Kupyna.cpp
        Lab3/Kupyna.h
        Lab4/MillerRabin.cpp
        Lab4/MillerRabin.h
        Lab4/RSA.cpp
        Lab4/RSA.h
        BigInt.cpp
        BigInt.h
)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(crypto ${SOURCE_FILES} main.cpp)

# Add googletest
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


set(
        TESTS
        test/test1.cpp
        test/test2.cpp
        test/test3.cpp
        test/test4.cpp
        test/big_int.cpp
        test/file_test.cpp
)

add_executable(crypto_unittest ${TESTS} ${SOURCE_FILES})

target_link_libraries(crypto_unittest PUBLIC gtest_main)
