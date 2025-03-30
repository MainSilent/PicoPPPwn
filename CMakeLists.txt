cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(HEADERS
    stage1.h
    stage2.h
    lib/offset.h
    lib/exploit.h
    lib/defines.h
    lib/wiznet/w5500.h
    lib/wiznet/socket.h
    lib/wiznet/wizchip_conf.h
)

set(SOURCES
    lib/packet.c
    lib/exploit.c
    lib/wiznet/w5500.c
    lib/wiznet/socket.c
    lib/wiznet/wizchip_conf.c
)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_pppwn C CXX ASM)

pico_sdk_init()

add_executable(pico_pppwn main.c ${SOURCES})

target_include_directories(pico_pppwn PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(pico_pppwn pico_stdlib hardware_spi)

pico_enable_stdio_usb(pico_pppwn 1)
pico_enable_stdio_uart(pico_pppwn 0)

pico_add_extra_outputs(pico_pppwn)