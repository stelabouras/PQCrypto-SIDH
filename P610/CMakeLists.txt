cmake_minimum_required(VERSION 3.5)


set (arith_src ${test_base}/arith_tests-p610.c ${test_base}/test_extras.c)
set(sidh_src ${test_base}/test_SIDHp610.c ${test_base}/test_extras.c)
set(sike_src ${test_base}/test_SIKEp610.c ${test_base}/test_extras.c)

set(sidh_comp_src ${test_base}/test_SIDHp610_compressed.c ${test_base}/test_extras.c)
set(sike_comp_src ${test_base}/test_SIKEp610_compressed.c ${test_base}/test_extras.c)

add_executable(arith_p610_test ${arith_src})
target_link_libraries(arith_p610_test sidh_610 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(arith_p610_test sidh_610)

add_executable(SIDH_610_test ${sidh_src})
target_link_libraries(SIDH_610_test sidh_610 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_610_test sidh_610)

add_executable(SIKE_610_test ${sike_src})
target_link_libraries(SIKE_610_test sidh_610 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_610_test sidh_610)

add_executable(SIDH_610_compressed_test ${sidh_comp_src})
target_link_libraries(SIDH_610_compressed_test sidh_610_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIDH_610_compressed_test sidh_610_comp)

add_executable(SIKE_610_compressed_test ${sike_comp_src})
target_link_libraries(SIKE_610_compressed_test sidh_610_comp ${LDFLAGS} ${ARM_SETTING})
add_dependencies(SIKE_610_compressed_test sidh_610_comp)

### KAT tests - set correct base directory, tests need to open KAT files
### when using Clion set the working directory of the run configuration to $PROJECT_DIR$

add_library (sidh_for_testing_610 ${LIBRARY_BUILD_TYPE} ${OBJECTS_610} ${AES_OBJS} ${common_src})
add_library (sidh_for_testing_comp_610 ${LIBRARY_BUILD_TYPE} ${OBJECTS_610_COMP} ${AES_OBJS} ${common_src})

add_executable(PQC_KAT_kem_610_test ${test_base}/PQCtestKAT_kem610.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_610_test sidh_for_testing_610 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_610_test sidh_for_testing_610)

add_executable(PQC_KAT_kem_610_comp_test ${test_base}/PQCtestKAT_kem610_compressed.c ${RNG_OBJS})
target_link_libraries(PQC_KAT_kem_610_comp_test sidh_for_testing_comp_610 ${LDFLAGS} ${ARM_SETTING})
add_dependencies(PQC_KAT_kem_610_comp_test sidh_for_testing_comp_610)