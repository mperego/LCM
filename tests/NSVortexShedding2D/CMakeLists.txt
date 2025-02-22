# Name the test with the directory name
get_filename_component(testNameRoot ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Copy mesh files from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vortex.exo
               ${CMAKE_CURRENT_BINARY_DIR}/vortex.exo COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vortex.exo.4.0
               ${CMAKE_CURRENT_BINARY_DIR}/vortex.exo.4.0 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vortex.exo.4.1
               ${CMAKE_CURRENT_BINARY_DIR}/vortex.exo.4.1 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vortex.exo.4.2
               ${CMAKE_CURRENT_BINARY_DIR}/vortex.exo.4.2 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vortex.exo.4.3
               ${CMAKE_CURRENT_BINARY_DIR}/vortex.exo.4.3 COPYONLY)

#
# Transient tests
#

set(testName ${testNameRoot}_TransientIRK)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputTransientRKT.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputTransientRKT.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/inputTransientRKT_RegressFail.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/inputTransientRKT_RegressFail.yaml COPYONLY)

add_test(${testName} ${Albany.exe} inputTransientRKT.yaml)
set_tests_properties(${testName} PROPERTIES LABELS "Demo;Tpetra;Forward")

add_test(NAME ${testName}_RegressFail
         COMMAND ${Albany.exe} inputTransientRKT_RegressFail.yaml)
set_tests_properties(${testName}_RegressFail PROPERTIES WILL_FAIL TRUE)
set_tests_properties(${testName}_RegressFail
                     PROPERTIES LABELS "Demo;Tpetra;Forward;RegressFail")
