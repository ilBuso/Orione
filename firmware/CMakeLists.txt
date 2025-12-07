# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# === Firmware configs ===
project(orione_firmware VERSION 1.0.0 LANGUAGES C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Platform independent sources
include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/init
        ${CMAKE_SOURCE_DIR}/src/matrix
        ${CMAKE_SOURCE_DIR}/src/message
)

file(GLOB_RECURSE INIT_SOURCES src/init/*.c)
file(GLOB_RECURSE MATRIX_SOURCES src/matrix/*.c)
file(GLOB_RECURSE MESSAGE_SOURCES src/message/*.c)

add_executable(firmware
        main.c
        ${INIT_SOURCES}
        ${MATRIX_SOURCES}
        ${MESSAGE_SOURCES}
)


# === Driver lib configs ===
set(DRIVERLIB_PATH "/Users/j/Developer/simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/driverlib")
set(MSP_INC_PATH "/Users/j/Developer/simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/inc")
set(MSP_SOURCES_PATH "/Users/j/Developer/simplelink_msp432p4_sdk_3_40_01_02/source")
set(MSP_DRIVERS_PATH "Users/j/Developer/simplelink_msp432p4_sdk_3_40_01_02/source/ti/drivers")

file(GLOB_RECURSE SDK_DRIVERS_INCLUDE_DIRS RELATIVE ${MSP_DRIVERS_PATH} ${MSP_DRIVERS_PATH}/*)

foreach(dir ${SDK_DRIVERS_INCLUDE_DIRS})
    if(IS_DIRECTORY ${MSP_DRIVERS_PATH}/${dir})
        target_include_directories(firmware PRIVATE ${MSP_DRIVERS_PATH}/${dir})
    endif()
endforeach()

target_include_directories(firmware PRIVATE ${MSP_SOURCES_PATH})
target_include_directories(firmware PRIVATE ${MSP_INC_PATH})
target_include_directories(firmware PRIVATE ${DRIVERLIB_PATH})
target_link_libraries(firmware PRIVATE ${DRIVERLIB_PATH}/gcc/msp432p4xx_driverlib.a)