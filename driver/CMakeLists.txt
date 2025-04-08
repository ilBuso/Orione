# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Orione-v1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include tomlc99 directory for headers
include_directories(TOMLC99)

# Add tmolc99 source
set(TOMLC99_SRC src/tomlc99/toml.c)

# Add executable targets, linking tomlc99
add_executable(driver-linux src/orionev1-linux-driver.c ${TOMLC99_SRC})
add_executable(driver-win64 src/orionev1-win64-driver.c ${TOMLC99_SRC})
add_executable(driver-macos src/orionev1-macos-driver.c ${TOMLC99_SRC})