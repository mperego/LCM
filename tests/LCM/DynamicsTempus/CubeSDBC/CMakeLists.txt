#
# Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
# Sandia, LLC (NTESS). This Software is released under the BSD license detailed
# in the file license.txt in the top-level Albany directory.
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube.g
               ${CMAKE_CURRENT_BINARY_DIR}/cube.g COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/material.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/material.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/cube.yaml COPYONLY)

get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_test(Serial_Dynamic_${testName}_NewmarkImplicitDForm_Tempus
         ${SerialAlbany.exe} cube.yaml)
set_tests_properties(Serial_Dynamic_${testName}_NewmarkImplicitDForm_Tempus
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
