#
# Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
# Sandia, LLC (NTESS). This Software is released under the BSD license detailed
# in the file license.txt in the top-level Albany directory.
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single.g
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single.g COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single-tet10.g
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single-tet10.g COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes.g
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes.g COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes.g.4.0
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes.g.4.0 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes.g.4.1
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes.g.4.1 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes.g.4.2
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes.g.4.2 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes.g.4.3
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes.g.4.3 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/materials-cubes.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/materials-cubes.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/materials-cubes-ct.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/materials-cubes-ct.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single-piro.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single-piro.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single-piro-expl.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single-piro-expl.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single-tempus.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single-tempus.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cube-single-ct-tempus.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/cube-single-ct-tempus.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cube-single-tempus-expl.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/cube-single-tempus-expl.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes-tempus.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes-tempus.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/larger-cubes-tempus-expl.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/larger-cubes-tempus-expl.yaml COPYONLY)

get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_test(Serial_Dynamic_${testName}_NewmarkImplicitAForm_Tempus
         ${SerialAlbany.exe} cube-single-tempus.yaml)
set_tests_properties(Serial_Dynamic_${testName}_NewmarkImplicitAForm_Tempus
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
add_test(Parallel_Dynamic_${testName}_NewmarkImplicitAForm_Tempus
         ${Albany8.exe} larger-cubes-tempus.yaml)
set_tests_properties(Parallel_Dynamic_${testName}_NewmarkImplicitAForm_Tempus
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
add_test(Parallel_Dynamic_${testName}_NewmarkExplicitAForm_LumpedMass_Tempus
         ${Albany8.exe} larger-cubes-tempus-expl.yaml)
set_tests_properties(
  Parallel_Dynamic_${testName}_NewmarkExplicitAForm_LumpedMass_Tempus
  PROPERTIES LABELS "LCM;Tpetra;Forward")
