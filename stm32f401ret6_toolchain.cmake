# https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)  # Bare-metal target
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb")

find_program(OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(ELF_SIZE arm-none-eabi-size REQUIRED)

add_compile_definitions(STM32F4 STM32F401xE)