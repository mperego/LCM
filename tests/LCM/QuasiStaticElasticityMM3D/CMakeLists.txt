# 1. Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# 1. Create the test with this name and standard executable

if(ALBANY_64BIT_INT)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.yaml
                 ${CMAKE_CURRENT_BINARY_DIR}/input.yaml COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/materials.yaml
                 ${CMAKE_CURRENT_BINARY_DIR}/materials.yaml COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/turbine.e
                 ${CMAKE_CURRENT_BINARY_DIR}/turbine.e COPYONLY)
  add_test(${testName}_64 ${Albany.exe} input.yaml)
  set_tests_properties(${testName}_64 PROPERTIES LABELS "LCM;Tpetra;Forward")
endif()
