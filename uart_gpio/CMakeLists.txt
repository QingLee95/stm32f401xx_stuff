get_filename_component(EXE ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project($EXE)

## Set paths.
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c)

add_executable(${EXE} ${STARTUP_CODE} main.c ${SOURCES})
target_include_directories(${EXE} PRIVATE ${INC_DIR})
target_link_options(${EXE} PRIVATE -T${LINKER_SCRIPT})