cmake_minimum_required(VERSION 3.13)

# pico_sdk_import.cmake is a single file copied from this SDK
include(external/pico-sdk/pico_sdk_init.cmake)

project(gfm_pico LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(external/BNO08x_Pico_Library)

add_subdirectory(src/modules/controllers/bno085)

add_executable(gfm_pico src/main.cpp src/usb_descriptors.c)
target_link_libraries(gfm_pico pico_stdlib bno085_controller tinyusb_device tinyusb_board)
target_include_directories(gfm_pico PRIVATE src/)

pico_enable_stdio_usb(gfm_pico 0)
pico_enable_stdio_uart(gfm_pico 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gfm_pico)
