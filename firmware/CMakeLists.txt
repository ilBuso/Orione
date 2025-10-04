# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(orione C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(orione 
        main.c 
        src/init/init.c 
        src/interrupts/interrupts.c
        src/matrix/keymap/keymap.c
        src/matrix/scan_rows/scan_rows.c
        src/rotary_encoder/rotary_encoder.c
        src/usb/usb_descriptors/usb_descriptors.c
        src/usb/usb_callbacks/usb_callbacks.c)

pico_set_program_name(orione "orione")
pico_set_program_version(orione "0.2")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(orione 0)
pico_enable_stdio_usb(orione 0)

target_sources(orione PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_descriptors/usb_descriptors.c
        )

# Add the standard library to the build
target_link_libraries(orione PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# Add the standard include files to the build
target_include_directories(orione PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/config
)

pico_add_extra_outputs(orione)