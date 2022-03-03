# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/syso/dev/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(debugomatic C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

# Add any PICO_CONFIG entries specified in the Advanced settings
add_compile_definitions(PICO_FLASH_SIZE_BYTES=16777216)

add_executable(debugomatic debugomatic.c )

pico_set_program_name(debugomatic "debugomatic")
pico_set_program_version(debugomatic "0.1")

pico_enable_stdio_uart(debugomatic 0)
pico_enable_stdio_usb(debugomatic 1)

# Add the standard library to the build
target_link_libraries(debugomatic pico_stdlib)

# Add any user requested libraries
target_link_libraries(debugomatic
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_timer
        hardware_watchdog
        hardware_clocks
        )

pico_add_extra_outputs(debugomatic)

