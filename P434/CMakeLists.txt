cmake_minimum_required(VERSION 3.5)



set (arith_src ${test_base}/arith_tests-p434.c ${test_base}/test_extras.c)
set(sidh_src ${test_base}/test_SIDHp434.c ${test_base}/test_extras.c)
set(sike_src ${test_base}/test_SIKEp434.c ${test_base}/test_extras.c)

set(sidh_comp_src ${test_base}/test_SIDHp434_compressed.c ${test_base}/test_extras.c)
set(sike_comp_src ${test_base}/test_SIKEp434_compressed.c ${test_base}/test_extras.c)

add_executable(arith_p434_test ${arith_src})
target_link_libraries(arith_p434_test sidh_434 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(arith_p434_test sidh_434)

add_executable(SIDH_434_test ${sidh_src})
target_link_libraries(SIDH_434_test sidh_434 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_434_test sidh_434)

add_executable(SIKE_434_test ${sike_src})
target_link_libraries(SIKE_434_test sidh_434 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_434_test sidh_434)

add_executable(SIDH_434_compressed_test ${sidh_comp_src})
target_link_libraries(SIDH_434_compressed_test sidh_434_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_434_compressed_test sidh_434_comp)

add_executable(SIKE_434_compressed_test ${sike_comp_src})
target_link_libraries(SIKE_434_compressed_test sidh_434_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_434_compressed_test sidh_434_comp)

### KAT tests - set correct base directory, tests need to open KAT files
### when using Clion set the working directory of the run configuration to $PROJECT_DIR$

add_library (sidh_for_testing_434 ${LIBRARY_BUILD_TYPE} ${OBJECTS_434} ${AES_OBJS})
add_library (sidh_for_testing_comp_434 ${LIBRARY_BUILD_TYPE} ${OBJECTS_434_COMP} ${AES_OBJS})

add_executable(PQC_KAT_kem_434_test ${test_base}/PQCtestKAT_kem434.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_434_test sidh_for_testing_434 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_434_test sidh_for_testing_434)

add_executable(PQC_KAT_kem_434_comp_test ${test_base}/PQCtestKAT_kem434_compressed.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_434_comp_test sidh_for_testing_comp_434 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_434_comp_test sidh_for_testing_comp_434)