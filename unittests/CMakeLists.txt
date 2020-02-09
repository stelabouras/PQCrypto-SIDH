cmake_minimum_required(VERSION 3.5)

#######
# Setup for tests using google test framework
#######

add_executable(sidh434_test SidhBasic434_test.cpp)
target_link_libraries(sidh434_test gtest_main ${sidhLibName})

add_executable(sidh503_test SidhBasic503_test.cpp)
target_link_libraries(sidh503_test gtest_main ${sidhLibName})

add_executable(sidh610_test SidhBasic610_test.cpp)
target_link_libraries(sidh610_test gtest_main ${sidhLibName})

add_executable(sidh751_test SidhBasic751_test.cpp)
target_link_libraries(sidh751_test gtest_main ${sidhLibName})
