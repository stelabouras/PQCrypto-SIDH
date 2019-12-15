cmake_minimum_required(VERSION 3.5)
project(SIDH)
enable_language(ASM)

set (VERSION 3.2.0)

# cmake -DARCH=x86|x64|arm|arm64 -DOPT_LEVEL_GENERIC=true|false

option(x64 "Set this to true to build for a x64 (64 bit) architecture." OFF)
option(x86 "Set this to true to build for a x86 (32 bit) architecture." OFF)
option(arm "Set this to true to build for a ARM (32 bit) architecture." OFF)
option(arm64 "Set this to true to build for a ARM (64 bit) architecture." OFF)

option(OPT_LEVEL_GENERIC "Set this to true to use the portable implementation." )

set(use_mulx true)
set(use_adx true)

set (OPT -O3)
set (CMAKE_C_COMPILER "clang")

if (NOT x64 AND NOT x86 AND NOT arm AND NOT arm64)
    message(FATAL_ERROR "No architecture selected.")
endif()

if (x64 AND (x86 OR arm OR arm64))
    message(FATAL_ERROR "You can select only one architecture.")
endif()

if (x86 AND (x64 OR arm OR arm64))
    message(FATAL_ERROR "You can select only one architecture.")
endif()

if (arm AND (x86 OR x64 OR arm64))
    message(FATAL_ERROR "You can select only one architecture.")
endif()

if (arm64 AND (x86 OR x64 OR arm))
    message(FATAL_ERROR "You can select only one architecture.")
endif()

if(x64)
    set(ARCHITECTURE _AMD64_)
    set(USE_OPT_LEVEL "_FAST_")
endif()

if(x86)
    set (ARCHITECTURE _X86_)
    set(USE_OPT_LEVEL "_GENERIC_")
endif()

if(arm)
    set(ARCHITECTURE _ARM_)
    set(USE_OPT_LEVEL "_GENERIC_")
    set(ARM_TARGET true)
endif()

if(arm64)
    set(ARCHITECTURE _ARM64_)
    set(ARM_TARGET true)
    set(USE_OPT_LEVEL "_FAST_")
endif()

if (OPT_LEVEL_GENERIC)
    set(USE_OPT_LEVEL "_GENERIC_")
endif()

if (ARM_TARGET)
   set(ARM_SETTING -lrt)
endif()

if (x64)
   if (${USE_OPT_LEVEL} STREQUAL "_FAST_")
      set (MULX "-D_MULX_")
      if (NOT use_mulx)
         set (MULX "")
      else()
         set(ADX "-D_ADX_")
         if (NOT use_adx)
            set (ADX "")
         endif()
      endif()
   endif()
endif()

set (LIBRARY_BUILD_TYPE "STATIC")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (SYSTEM "__LINUX__")
endif()
set (ADDITIONAL_SETTING "-march=native")

if (CMAKE_C_COMPILER STREQUAL "clang")
   if (ARM_TARGET)
      set(ADDITIONAL_SETTINGS "")
   endif()
endif()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADDITIONAL_SETTING} ${OPT} -D${ARCHITECTURE} -D${USE_OPT_LEVEL} -D${SYSTEM} ${MULX} ${ADX}")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" )
set (LDFLAGS -lm)

MESSAGE(STATUS "flags: ${CMAKE_C_FLAGS}")

set(src_base ${CMAKE_SOURCE_DIR}/src)
set(test_base ${CMAKE_SOURCE_DIR}/tests)

if (${USE_OPT_LEVEL} STREQUAL "_GENERIC")
    set(EXTRA_OBJECTS_434 ${src_base}/P434/generic/fp_generic.c)
    set(EXTRA_OBJECTS_503 ${src_base}/P503/generic/fp_generic.c)
    set(EXTRA_OBJECTS_610 ${src_base}/P610/generic/fp_generic.c)
    set(EXTRA_OBJECTS_751 ${src_base}/P751/generic/fp_generic.c)
elseif(${USE_OPT_LEVEL} STREQUAL "_FAST_")
    if (x64)
        set(EXTRA_OBJECTS_434 ${src_base}/P434/AMD64/fp_x64.c ${src_base}/P434/AMD64/fp_x64_asm.S)
        set(EXTRA_OBJECTS_503 ${src_base}/P503/AMD64/fp_x64.c ${src_base}/P503/AMD64/fp_x64_asm.S)
        set(EXTRA_OBJECTS_610 ${src_base}/P610/AMD64/fp_x64.c ${src_base}/P610/AMD64/fp_x64_asm.S)
        set(EXTRA_OBJECTS_751 ${src_base}/P751/AMD64/fp_x64.c ${src_base}/P751/AMD64/fp_x64_asm.S)
    elseif(arm64)
        set(EXTRA_OBJECTS_434 ${src_base}/P434/generic/fp_generic.c)
        set(EXTRA_OBJECTS_503 ${src_base}/P503/ARM64/fp_arm64.c ${src_base}/P503/ARM64/fp_arm64_asm.S)
        set(EXTRA_OBJECTS_610 ${src_base}/P610/generic/fp_generic.c)
        set(EXTRA_OBJECTS_751 ${src_base}/P751/ARM64/fp_arm64.c ${src_base}/P751/ARM64/fp_arm64_asm.S)
    endif()
endif()

set(OBJECTS_434 ${src_base}/P434/P434.c ${EXTRA_OBJECTS_434} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_503 ${src_base}/P503/P503.c ${EXTRA_OBJECTS_503} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_610 ${src_base}/P610/P610.c ${EXTRA_OBJECTS_610} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_751 ${src_base}/P751/P751.c ${EXTRA_OBJECTS_751} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_434_COMP ${src_base}/P434/P434_compressed.c ${EXTRA_OBJECTS_434} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_503_COMP ${src_base}/P503/P503_compressed.c ${EXTRA_OBJECTS_503} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_610_COMP ${src_base}/P610/P610_compressed.c ${EXTRA_OBJECTS_610} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)
set(OBJECTS_751_COMP ${src_base}/P751/P751_compressed.c ${EXTRA_OBJECTS_751} ${src_base}/random/random.c ${src_base}/sha3/fips202.c)

set(AES_OBJS ${test_base}/aes/aes.c ${test_base}/aes/aes_c.c)
set(RNG_OBJS ${test_base}/rng/rng.c)

add_subdirectory(P434)
add_subdirectory(P503)
add_subdirectory(P610)
add_subdirectory(P751)
