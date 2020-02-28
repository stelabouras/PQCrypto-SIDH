cmake_minimum_required(VERSION 3.5)

# Set names of Android SIDH shared and static library
set (android_lib androidSidh)
set (android_lib_static androidSidhStatic)

add_library(${android_lib} SHARED ${cpp_lib_src})
target_link_libraries(${android_lib} sidh_434 sidh_503 sidh_610 sidh_751 log)
# The SIDH wrapper sources require the following external include paths to find the external *.h files
target_include_directories(${android_lib} PRIVATE ${CMAKE_SOURCE_DIR}/src )

add_library(${android_lib_static} STATIC ${cpp_lib_src})
target_link_libraries(${android_lib_static} sidh_434 sidh_503 sidh_610 sidh_751)
target_include_directories(${android_lib_static} PRIVATE ${CMAKE_SOURCE_DIR}/src )
