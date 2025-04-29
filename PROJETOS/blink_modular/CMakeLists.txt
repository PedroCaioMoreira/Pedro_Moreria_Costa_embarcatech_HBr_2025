cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(blink_indefinido)

pico_sdk_init()

add_executable(blink_indefinido
    app/main.c
    hal/hal_led.c
    drivers/led_embutido.c
)

# Inclui os headers de include/
target_include_directories(blink_indefinido PRIVATE include)

# Linka bibliotecas do Pico (STD e CYW43 sem Wi-Fi)
target_link_libraries(blink_indefinido PRIVATE pico_stdlib pico_cyw43_arch_none)

# Gera arquivos UF2, map, bin, hex, etc.
pico_add_extra_outputs(blink_indefinido)
