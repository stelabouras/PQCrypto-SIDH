cmake_minimum_required(VERSION 3.5)

# Set name of the Android SIDH shared library
set (android_lib android_sidh)

add_library(a_sidh_434 STATIC IMPORTED)
set_target_properties(android_sidh_434 PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/android-sidh/build/intermediates/cmake/debug/obj/${ANDROID_ABI}/libsidh_434.a)

add_library(${android_lib} SHARED ${cpp_lib_src})
add_dependencies(${android_lib} sidh_434)

# The chatlib sources require the following external include paths to find the external *.h files
target_include_directories(${android_lib} PRIVATE ${CMAKE_SOURCE_DIR}/src )

target_link_libraries(${android_lib}
        a_sidh_434
        log
        )
