# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(orione_driver VERSION 1.0.0 LANGUAGES C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Platform independent sources
include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/core
        ${CMAKE_SOURCE_DIR}/src/emulator
        ${CMAKE_SOURCE_DIR}/src/global
        ${CMAKE_SOURCE_DIR}/src/matrix
        ${CMAKE_SOURCE_DIR}/src/message
        ${CMAKE_SOURCE_DIR}/src/profile
        ${CMAKE_SOURCE_DIR}/src/ratelimiter
)

file(GLOB_RECURSE CORE_SOURCES src/core/*.c)
file(GLOB_RECURSE EMULATOR_SOURCES src/archive/emulator/*.c)
file(GLOB_RECURSE GLOBAL_SOURCES src/global/*.c)
file(GLOB_RECURSE MATRIX_SOURCES src/matrix/*.c)
file(GLOB_RECURSE MESSAGE_SOURCES src/message/*.c)
file(GLOB_RECURSE PROFILE_SOURCES src/profile/*.c)
file(GLOB_RECURSE RATELIMITER_SOURCES src/ratelimiter/*.c)

file(GLOB_RECURSE PLATFORM_SOURCES platform/linux/*.c)
add_definitions(-DPLATFORM_LINUX)
include_directories(${CMAKE_SOURCE_DIR}/platform/linux)

add_executable(driver
        src/main.c
        ${CORE_SOURCES}
        ${EMULATOR_SOURCES}
        ${GLOBAL_SOURCES}
        ${MATRIX_SOURCES}
        ${MESSAGE_SOURCES}
        ${PROFILE_SOURCES}
        ${RATELIMITER_SOURCES}
        ${PLATFORM_SOURCES}
)

# Configuration macros

# Logging
set(LOG_LEVEL "DEBUG" CACHE STRING "Choose the log level: DEBUG, INFO, WARNING, ERROR")

if(LOG_LEVEL STREQUAL "DEBUG")
    add_definitions(-DLOG_LEVEL_DEBUG)
elseif(LOG_LEVEL STREQUAL "INFO")
    add_definitions(-DLOG_LEVEL_INFO)
elseif(LOG_LEVEL STREQUAL "WARNING")
    add_definitions(-DLOG_LEVEL_WARNING)
elseif(LOG_LEVEL STREQUAL "ERROR")
    add_definitions(-DLOG_LEVEL_ERROR)
else()
    message(FATAL_ERROR "Invalid log level specified!")
endif()

# UART
set(LINUX_SERIAL_PORT "/dev/ttyUSB0" CACHE STRING "Path to serial port device")
target_compile_definitions(driver PRIVATE LINUX_SERIAL_PORT=\"${LINUX_SERIAL_PORT}\")

set(MACOS_SERIAL_PORT "/dev/ttyUSB0" CACHE STRING "Path to serial port device")
target_compile_definitions(driver PRIVATE MACOS_SERIAL_PORT=\"${MACOS_SERIAL_PORT}\")