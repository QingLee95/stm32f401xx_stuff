get_filename_component(APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${APP_NAME})

set(EXE ${PROJECT_NAME}.elf)
## Set paths.
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c)

add_executable(${EXE} ${STARTUP_CODE} main.c ${SOURCES})
target_include_directories(${EXE} PRIVATE ${INC_DIR})
target_link_options(${EXE} PRIVATE -T${LINKER_SCRIPT})

add_custom_command(TARGET ${EXE} POST_BUILD
    COMMAND ${OBJCOPY} -O binary ${EXE} ${PROJECT_NAME}.bin
    COMMAND ${ELF_SIZE} ${EXE}
)