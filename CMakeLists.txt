cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED PICO_TOOLCHAIN_PATH)
    set(PICO_TOOLCHAIN_PATH "/Applications/ArmGNUToolchain/15.2.rel1/arm-none-eabi/bin")
endif()

if(NOT DEFINED PICO_BOARD)
    set(PICO_BOARD "pimoroni_tiny2040")
endif()

include(pico_sdk_import.cmake)

project(CatGenieEmulator C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(CatGenieEmulator
        main.c
        config.h
        SRIX4K-A4S_1GE_memory_emulator.h
        types.h
        flash_memory.c
        flash_memory.h
)

target_link_libraries(CatGenieEmulator
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_gpio
        hardware_irq
        hardware_pio

)

pico_add_extra_outputs(CatGenieEmulator)

pico_enable_stdio_usb(CatGenieEmulator 1)
pico_enable_stdio_uart(CatGenieEmulator 0)