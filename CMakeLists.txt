cmake_minimum_required(VERSION 3.25)
# https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(stm32f401ret6)

set(CMAKE_SYSTEM_NAME Generic)  # Bare-metal target
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -nostdlib -Wall -Wextra -Werror -s")
message("Compiler cflags: ${CMAKE_C_FLAGS}.")

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/stm32f401ret6_flash.ld)
set(STARTUP_CODE ${CMAKE_CURRENT_SOURCE_DIR}/stm32f401ret6_startup.c)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(CMSIS ${LIBS}/CMSIS)
set(LIBC_IMPL ${LIBS}/libc_impl)

#Create own libc implementation because of -nostdlib and the use of own startup code and linker script
file(GLOB_RECURSE LIBC_SOURCES ${LIBC_IMPL}/*.c)
add_library(myc STATIC ${LIBC_SOURCES})

add_compile_definitions(STM32F4 STM32F401xE)
include_directories(${CMSIS}/Core/ ${CMSIS}/Device/)
link_libraries(myc)

add_subdirectory(uart_gpio)
add_subdirectory(systick_uart)
