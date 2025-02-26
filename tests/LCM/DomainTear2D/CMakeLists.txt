# 1. Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputBase.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputBase.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputCR.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputCR.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputTM.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputTM.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rip.exo
               ${CMAKE_CURRENT_BINARY_DIR}/rip.exo COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rip.exo.4.0
               ${CMAKE_CURRENT_BINARY_DIR}/rip.exo.4.0 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rip.exo.4.1
               ${CMAKE_CURRENT_BINARY_DIR}/rip.exo.4.1 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rip.exo.4.2
               ${CMAKE_CURRENT_BINARY_DIR}/rip.exo.4.2 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rip.exo.4.3
               ${CMAKE_CURRENT_BINARY_DIR}/rip.exo.4.3 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/materials.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/materials.yaml COPYONLY)

# 1. Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 1. Create the test with this name and standard executable
add_test(${testName}_BaseProblem ${Albany.exe} inputBase.yaml)
set_tests_properties(${testName}_BaseProblem PROPERTIES LABELS
                                                        "LCM;Tpetra;Forward")

add_test(${testName}_CopyRemesh ${Albany.exe} inputCR.yaml)
set_tests_properties(${testName}_CopyRemesh PROPERTIES LABELS
                                                       "LCM;Tpetra;Forward")
# add_test(${testName}_TopMod ${Albany.exe} inputM.yaml)
