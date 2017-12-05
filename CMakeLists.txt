set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_VERSION "1.1")
set(CMAKE_SYSTEM_PROCESSOR "MIPS64")

if(NOT "${CHERI_SDK}")
  set(CHERI_SDK $ENV{CHERI_SDK})
endif()
set(CHERI_SDK "${CHERI_SDK}" CACHE STRING "Path to cheri sdk" FORCE)
if(NOT EXISTS ${CHERI_SDK})
  message(FATAL_ERROR "YOU MUST SET $CHERI_SDK to a valid path. Was: ${CHERI_SDK}")
endif()

message(STATUS "CHERI SDK = ${CHERI_SDK}")


set(triple mips64-qemu-elf)

set(CMAKE_C_COMPILER "${CHERI_SDK}/clang")
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER "${CHERI_SDK}/clang++")
set(CMAKE_CXX_COMPILER_TARGET ${triple})

set(CMAKE_SYSROOT "${CHERI_SDK}/../baremetal/${triple}")

set(CMAKE_AR "${CHERI_SDK}/ar" CACHE FILEPATH "ar")
set(CMAKE_OBJDUMP "${CHERI_SDK}/objdump" CACHE FILEPATH "objdump")
set(CMAKE_OBJCOPY "${CHERI_SDK}/objcopy" CACHE FILEPATH "objcopy")
set(CMAKE_RANLIB "${CHERI_SDK}/ranlib" CACHE FILEPATH "ranlib")

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXE_LINKER_FLAGS_INIT    "-static -Wl,-T,qemu-malta.ld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-error-shared-libraries-won't-work")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-static -Wl,-T,qemu-malta.ld")

# appears to be needed for some reason
set(_common_flags "-D__ELF__ -D_GNU_SOURCE -fno-pic -mno-abicalls")
set(CMAKE_C_FLAGS_INIT   "${_common_flags}")
set(CMAKE_ASM_FLAGS_INIT "${_common_flags}")
set(CMAKE_CXX_FLAGS_INIT "${_common_flags} -fno-rtti -fno-exceptions")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


cmake_minimum_required(VERSION 3.8)
project(BaremetalTests LANGUAGES C CXX)

message(STATUS "Crosscompiling=${CMAKE_CROSSCOMPILING}, target=${CMAKE_SYSTEM}, CC=${CMAKE_C_COMPILER}")
if(NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "THIS NEEDS TO BE CROSS COMPILED!")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(QEMU_CHERI ${CHERI_SDK}/qemu-system-cheri)

# 100k instructions should be enough to run these tests
set(QEMU_BREAKPOINT_ARGS -bp 0xffffffff80000080 -bc 100000)

enable_testing()

function(add_qemu_test _binary)
  set(TRACE_FLAG -D ./trace.log -d instr)
  add_test(NAME ${_binary}_test COMMAND ${QEMU_CHERI} -M malta -kernel $<TARGET_FILE:${_binary}> -nographic ${QEMU_BREAKPOINT_ARGS})
endfunction()

function(add_test_binary _target) # _srcs
  add_executable(${_target} ${ARGN})
  add_qemu_test(${_target})
endfunction()

add_test_binary(helloworld helloworld.c)

add_test_binary(helloworld-cxx helloworld.cpp)

add_test_binary(argv-test argc-argv-envp.c)

