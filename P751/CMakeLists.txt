cmake_minimum_required(VERSION 3.5)

add_library (sidh_751 ${LIBRARY_BUILD_TYPE} ${OBJECTS_751})
add_library (sidh_751_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_751_COMP})


set (arith_src ${test_base}/arith_tests-p751.c ${test_base}/test_extras.c)
set(sidh_src ${test_base}/test_SIDHp751.c ${test_base}/test_extras.c)
set(sike_src ${test_base}/test_SIKEp751.c ${test_base}/test_extras.c)

set(sidh_comp_src ${test_base}/test_SIDHp751_compressed.c ${test_base}/test_extras.c)
set(sike_comp_src ${test_base}/test_SIKEp751_compressed.c ${test_base}/test_extras.c)

add_executable(arith_p751_test ${arith_src})
target_link_libraries(arith_p751_test sidh_751 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(arith_p751_test sidh_751)

add_executable(SIDH_751_test ${sidh_src})
target_link_libraries(SIDH_751_test sidh_751 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_751_test sidh_751)

add_executable(SIKE_751_test ${sike_src})
target_link_libraries(SIKE_751_test sidh_751 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_751_test sidh_751)

add_executable(SIDH_751_compressed_test ${sidh_comp_src})
target_link_libraries(SIDH_751_compressed_test sidh_751_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_751_compressed_test sidh_751_comp)

add_executable(SIKE_751_compressed_test ${sike_comp_src})
target_link_libraries(SIKE_751_compressed_test sidh_751_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_751_compressed_test sidh_751_comp)

### KAT tests - set correct base directory, tests need to open KAT files
### when using Clion set the working directory of the run configuration to $PROJECT_DIR$

add_library (sidh_for_testing_751 ${LIBRARY_BUILD_TYPE} ${OBJECTS_751} ${AES_OBJS})
add_library (sidh_for_testing_comp_751 ${LIBRARY_BUILD_TYPE} ${OBJECTS_751_COMP} ${AES_OBJS})

add_executable(PQC_KAT_kem_751_test ${test_base}/PQCtestKAT_kem751.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_751_test sidh_for_testing_751 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_751_test sidh_for_testing_751)

add_executable(PQC_KAT_kem_751_comp_test ${test_base}/PQCtestKAT_kem751_compressed.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_751_comp_test sidh_for_testing_comp_751 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_751_comp_test sidh_for_testing_comp_751)