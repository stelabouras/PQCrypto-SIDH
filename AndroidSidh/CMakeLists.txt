cmake_minimum_required(VERSION 3.5)

# Set name of the Android SIDH shared library
set (android_lib androidSidh)

add_library(${android_lib} SHARED ${cpp_lib_src})

# The SIDH wrapper sources require the following external include paths to find the external *.h files
target_include_directories(${android_lib} PRIVATE ${CMAKE_SOURCE_DIR}/src )

target_link_libraries(${android_lib} sidh_434 sidh_503 sidh_610 sidh_751 log)
