cmake_minimum_required(VERSION 3.5)

add_library (sidh_503 ${LIBRARY_BUILD_TYPE} ${OBJECTS_503})
add_library (sidh_503_comp ${LIBRARY_BUILD_TYPE} ${OBJECTS_503_COMP})


set (arith_src ${test_base}/arith_tests-p503.c ${test_base}/test_extras.c)
set(sidh_src ${test_base}/test_SIDHp503.c ${test_base}/test_extras.c)
set(sike_src ${test_base}/test_SIKEp503.c ${test_base}/test_extras.c)

set(sidh_comp_src ${test_base}/test_SIDHp503_compressed.c ${test_base}/test_extras.c)
set(sike_comp_src ${test_base}/test_SIKEp503_compressed.c ${test_base}/test_extras.c)

add_executable(arith_p503_test ${arith_src})
target_link_libraries(arith_p503_test sidh_503 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(arith_p503_test sidh_503)

add_executable(SIDH_503_test ${sidh_src})
target_link_libraries(SIDH_503_test sidh_503 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_503_test sidh_503)

add_executable(SIKE_503_test ${sike_src})
target_link_libraries(SIKE_503_test sidh_503 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_503_test sidh_503)

add_executable(SIDH_503_compressed_test ${sidh_comp_src})
target_link_libraries(SIDH_503_compressed_test sidh_503_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_503_compressed_test sidh_503_comp)

add_executable(SIKE_503_compressed_test ${sike_comp_src})
target_link_libraries(SIKE_503_compressed_test sidh_503_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_503_compressed_test sidh_503_comp)

### KAT tests - set correct base directory, tests need to open KAT files
### when using Clion set the working directory of the run configuration to $PROJECT_DIR$

add_library (sidh_for_testing_503 ${LIBRARY_BUILD_TYPE} ${OBJECTS_503} ${AES_OBJS})
add_library (sidh_for_testing_comp_503 ${LIBRARY_BUILD_TYPE} ${OBJECTS_503_COMP} ${AES_OBJS})

add_executable(PQC_KAT_kem_503_test ${test_base}/PQCtestKAT_kem503.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_503_test sidh_for_testing_503 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_503_test sidh_for_testing_503)

add_executable(PQC_KAT_kem_503_comp_test ${test_base}/PQCtestKAT_kem503_compressed.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_503_comp_test sidh_for_testing_comp_503 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_503_comp_test sidh_for_testing_comp_503)