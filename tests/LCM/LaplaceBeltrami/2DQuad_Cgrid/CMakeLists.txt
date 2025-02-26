if(SEACAS_EPU AND SEACAS_EXODIFF)
  # 1. Copy Input file from source to binary dir
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.yaml
                 ${CMAKE_CURRENT_BINARY_DIR}/input.yaml COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputPS.yaml
                 ${CMAKE_CURRENT_BINARY_DIR}/inputPS.yaml COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputLB.yaml
                 ${CMAKE_CURRENT_BINARY_DIR}/inputLB.yaml COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2d_cgrid_quad4.e
                 ${CMAKE_CURRENT_BINARY_DIR}/2d_cgrid_quad4.e COPYONLY)

  # 1. Name the test with the directory name
  get_filename_component(testDirName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  # 1. Create the test with this name and standard executable

  set(testName ${testDirName})
  add_test(
    NAME LaplaceBeltrami:${testName}
    COMMAND
      ${CMAKE_COMMAND} "-DTEST_PROG=${Albany.exe}" -DTEST_NAME=${testName}
      -DTEST_ARGS= -DMPIMNP=${MPIMNP} -DSEACAS_EPU=${SEACAS_EPU}
      -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/runtest.cmake)
  set_tests_properties(LaplaceBeltrami:${testName}
                       PROPERTIES LABELS "LCM;Tpetra;Forward")

  set(testName ${testDirName}_TPS)
  add_test(
    NAME LaplaceBeltrami:${testName}
    COMMAND
      ${CMAKE_COMMAND} "-DTEST_PROG=${Albany.exe}" -DTEST_NAME=${testName}
      -DTEST_ARGS=inputPS.yaml -DMPIMNP=${MPIMNP} -DSEACAS_EPU=${SEACAS_EPU}
      -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/runtest.cmake)
  set_tests_properties(LaplaceBeltrami:${testName}
                       PROPERTIES LABELS "LCM;Tpetra;Forward")

  set(testName ${testDirName}_LB)
  add_test(
    NAME LaplaceBeltrami:${testName}
    COMMAND
      ${CMAKE_COMMAND} "-DTEST_PROG=${Albany.exe}" -DTEST_NAME=${testName}
      -DTEST_ARGS=inputLB.yaml -DMPIMNP=${MPIMNP} -DSEACAS_EPU=${SEACAS_EPU}
      -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/runtest.cmake)
  set_tests_properties(LaplaceBeltrami:${testName}
                       PROPERTIES LABELS "LCM;Tpetra;Forward")
endif()
