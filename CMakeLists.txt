cmake_minimum_required(VERSION 3.10)
project(FlatHeadBro)
enable_language( C ASM )
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_OBJCOPY riscv64-unknown-elf-objcopy)
set(CMAKE_OBJDUMP riscv64-unknown-elf-objdump)
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/os.ld")
add_compile_options(-nostdlib -lgcc )
file(GLOB SRC  ${CMAKE_CURRENT_SOURCE_DIR}/code/*.c ${CMAKE_CURRENT_SOURCE_DIR}/code/*.S)
set(INCLUDE_DIR)

add_subdirectory(code/base)
add_subdirectory(code/uart)
add_subdirectory(code/printf)
add_subdirectory(code/malloc)
add_subdirectory(code/switch)
add_subdirectory(code/interrupt)
include_directories(${INCLUDE_DIR})


add_executable(susuos.elf ${SRC})
target_link_libraries(susuos.elf  -nostdlib -lgcc)
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_custom_target(susuos.bin ALL 
COMMAND ${CMAKE_OBJCOPY} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}susuos.elf" -O binary "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}susuos.bin"
DEPENDS susuos.elf)
add_custom_target(susuos.list ALL 
COMMAND ${CMAKE_OBJDUMP} -D -b binary -m riscv  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}susuos.bin" > ${CMAKE_BINARY_DIR}/susuos.map
DEPENDS susuos.bin)

add_custom_target(susuoself.map ALL 
COMMAND ${CMAKE_OBJDUMP} -D -Mnumeric,no-aliases  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}susuos.elf" > ${CMAKE_BINARY_DIR}/susuoself.map
DEPENDS susuos.elf)

