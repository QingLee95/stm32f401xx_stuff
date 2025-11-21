cmake_minimum_required(VERSION 3.25)
project(stm32f401ret6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -nostdlib -O0 -Wall -Wextra -Werror -s")
message("Compiler cflags: ${CMAKE_C_FLAGS}.")

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/stm32f401ret6_flash.ld)
set(STARTUP_CODE ${CMAKE_CURRENT_SOURCE_DIR}/stm32f401ret6_startup.c)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(CMSIS ${LIBS}/CMSIS)
set(LIBC_IMPL ${LIBS}/libc_impl)

#Create own libc implementation because of -nostdlib and the use of own startup code and linker script
file(GLOB_RECURSE LIBC_SOURCES ${LIBC_IMPL}/*.c)
add_library(myc STATIC ${LIBC_SOURCES})

include_directories(${CMSIS}/Core/ ${CMSIS}/Device/)
link_libraries(myc)

add_subdirectory(uart_gpio)
add_subdirectory(systick_uart)
