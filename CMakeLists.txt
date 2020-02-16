cmake_minimum_required(VERSION 3.5)
project(SIDH)
enable_language(ASM)

set (VERSION 3.2.0)

# cmake -Dx86=true|x64=true|arm=true|arm64=true -DOPT_LEVEL_GENERIC=true|false

option(x64 "Set this to true to build for a x64 (64 bit) architecture." OFF)
option(x86 "Set this to true to build for a x86 (32 bit) architecture." OFF)
option(arm "Set this to true to build for a ARM (32 bit) architecture." OFF)
option(arm64 "Set this to true to build for a ARM (64 bit) architecture." OFF)

option(OPT_LEVEL_GENERIC "Set this to true to use the portable implementation." )

if (ANDROID)
    if (ANDROID_ABI STREQUAL "armeabi-v7a")
        set (arm true)
    elseif(ANDROID_ABI STREQUAL "arm64-v8a")
        set(arm64 true)
    elseif(ANDROID_ABI STREQUAL "x86")
        set(x86 true)
    elseif(ANDROID_ABI STREQUAL "x86_64")
        set(x64 true)
    else()
        message(FATAL_ERROR "Unknown Android ABI: ${ANDROID_ABI}.")
    endif()
endif()

set(G_TESTS true)    # enable google Test framework and unit tests

set(use_mulx true)
set(use_adx true)

set (OPT -O3)

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
#    set(USE_OPT_LEVEL "_FAST_")    # Disable __FAST_, X64 assembly code is not position independent
    set(USE_OPT_LEVEL "_GENERIC_")

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

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR ANDROID)
    set (SYSTEM "__LINUX__")
endif()

set (ADDITIONAL_SETTING "-march=native")

set(clang_found "-1")
string(FIND ${CMAKE_C_COMPILER} "clang" clang_found REVERSE)

if (NOT clang_found STREQUAL "-1")
   if (ARM_TARGET)
      set(ADDITIONAL_SETTING "")
   endif()
endif()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC ${ADDITIONAL_SETTING} ${OPT} -D${ARCHITECTURE} -D${USE_OPT_LEVEL} -D${SYSTEM} ${MULX} ${ADX}")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" )
set (LDFLAGS -lm)

MESSAGE(STATUS "flags: ${CMAKE_C_FLAGS}")

set(src_base ${CMAKE_SOURCE_DIR}/src)
set(test_base ${CMAKE_SOURCE_DIR}/tests)

set(common_src
        ${src_base}/common/common_funcs.h
        ${src_base}/common/common_funcs.c)

if (${USE_OPT_LEVEL} STREQUAL "_GENERIC_")
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

add_library (sidh_434 ${LIBRARY_BUILD_TYPE} ${OBJECTS_434} ${common_src})
add_library (sidh_434_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_434_COMP} ${common_src})

add_library (sidh_503 ${LIBRARY_BUILD_TYPE} ${OBJECTS_503} ${common_src})
add_library (sidh_503_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_503_COMP} ${common_src})

add_library (sidh_610 ${LIBRARY_BUILD_TYPE} ${OBJECTS_610} ${common_src})
add_library (sidh_610_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_610_COMP} ${common_src})

add_library (sidh_751 ${LIBRARY_BUILD_TYPE} ${OBJECTS_751} ${common_src})
add_library (sidh_751_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_751_COMP} ${common_src})


add_subdirectory(P434)
add_subdirectory(P503)
add_subdirectory(P610)
add_subdirectory(P751)

set(sidhLibName sidhCppLib)
set(cpp_lib_src
        ${CMAKE_SOURCE_DIR}/cpp/SidhWrapper.h
        ${CMAKE_SOURCE_DIR}/cpp/SidhWrapper.cpp
        ${CMAKE_SOURCE_DIR}/cpp/SidhP434Wrapper.cpp
        ${CMAKE_SOURCE_DIR}/cpp/SidhP503Wrapper.cpp
        ${CMAKE_SOURCE_DIR}/cpp/SidhP610Wrapper.cpp
        ${CMAKE_SOURCE_DIR}/cpp/SidhP751Wrapper.cpp
        )

find_package(JNI)
if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
    include_directories(${JNI_INCLUDE_DIRS})

endif()

if (JNI_FOUND OR ANDROID)
    set(cpp_lib_src ${cpp_lib_src}
        ${CMAKE_SOURCE_DIR}/cpp/javaNative/sidhjava_test_SidhNativeTests.h
        ${CMAKE_SOURCE_DIR}/cpp/javaNative/sidhjava_SidhNative_FieldLengths.h
        ${CMAKE_SOURCE_DIR}/cpp/javaNative/sidhjava_SidhNative.h
        ${CMAKE_SOURCE_DIR}/cpp/javaNative/SidhNative.cpp
        )
endif ()

if (G_TESTS)
# Definitions to compile and link tests to the Java Native interface
add_definitions(-DADD_TESTS)
set(test_embedded ${CMAKE_SOURCE_DIR}/testsEmbedded)
set(test_srcs
        ${test_base}/test_extras.h
        ${test_base}/test_extras.c
        ${test_embedded}/arith_tests-p434.c
        ${test_embedded}/arith_tests-p503.c
        ${test_embedded}/arith_tests-p610.c
        ${test_embedded}/arith_tests-p751.c
        ${test_embedded}/test_SIDHp434.c
        ${test_embedded}/test_SIDHp503.c
        ${test_embedded}/test_SIDHp610.c
        ${test_embedded}/test_SIDHp751.c
        )

set(cpp_lib_src ${cpp_lib_src} ${test_srcs})
endif()

if (G_TESTS AND NOT ANDROID)
    # Download and unpack googletest at configure time.
    configure_file(CMakeLists_gtest.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
            EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    add_subdirectory(unittests)

endif()

add_library(${sidhLibName} SHARED ${cpp_lib_src})
target_link_libraries(${sidhLibName} PUBLIC sidh_434 sidh_503 sidh_610 sidh_751)
target_include_directories(${sidhLibName} PRIVATE ${CMAKE_SOURCE_DIR}/src )

if (ANDROID)
    message(STATUS "Configure for Android Configuration ${ANDROID_ABI}.")
    SET(SYS_TARGET Android-${ANDROID_ABI})
    add_subdirectory(AndroidSidhLib)
endif()