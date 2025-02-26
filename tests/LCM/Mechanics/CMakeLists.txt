#
# Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
# Sandia, LLC (NTESS). This Software is released under the BSD license detailed
# in the file license.txt in the top-level Albany directory.
#

# Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/input.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2materials.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/2materials.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/2hex.g
               ${CMAKE_CURRENT_BINARY_DIR}/2hex.g COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nleltri2d_tpetra_in.exo.4.0
               ${CMAKE_CURRENT_BINARY_DIR}/nleltri2d_tpetra_in.exo.4.0 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nleltri2d_tpetra_in.exo.4.1
               ${CMAKE_CURRENT_BINARY_DIR}/nleltri2d_tpetra_in.exo.4.1 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nleltri2d_tpetra_in.exo.4.2
               ${CMAKE_CURRENT_BINARY_DIR}/nleltri2d_tpetra_in.exo.4.2 COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nleltri2d_tpetra_in.exo.4.3
               ${CMAKE_CURRENT_BINARY_DIR}/nleltri2d_tpetra_in.exo.4.3 COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# test for 2D with blocked ordering
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputBlocked.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputBlocked.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputBlocked_dir_field.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputBlocked_dir_field.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/materials.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/materials.yaml COPYONLY)

# Create the test with this name and standard executable
add_test(${testName}2D_Blocked ${Albany.exe} inputBlocked.yaml)
set_tests_properties(${testName}2D_Blocked PROPERTIES LABELS
                                                      "LCM;Tpetra;Forward")
add_test(${testName}2D_Blocked_dir_field ${Albany.exe}
         inputBlocked_dir_field.yaml)
set_tests_properties(${testName}2D_Blocked_dir_field
                     PROPERTIES LABELS "LCM;Tpetra;Forward")

# test for 2D with J2 plasticity model
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputJ2Plasticity2D.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputJ2Plasticity2D.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputJ2Plast2DTraction.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputJ2Plast2DTraction.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/J2.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/J2.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/PlasticityJ2_2D_Traction.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/PlasticityJ2_2D_Traction.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/PlasticityJ2_2D_Traction_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/PlasticityJ2_2D_Traction_Material.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/PlasticityJ2_3D_Traction.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/PlasticityJ2_3D_Traction.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/PlasticityJ2_3D_Traction_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/PlasticityJ2_3D_Traction_Material.yaml COPYONLY)

# Create the test with this name and standard executable
add_test(${testName}2D_J2 ${Albany.exe} inputJ2Plasticity2D.yaml)
set_tests_properties(${testName}2D_J2 PROPERTIES LABELS "LCM;Tpetra;Forward")
add_test(${testName}_PlasticityJ2_2D_Traction ${Albany.exe}
         PlasticityJ2_2D_Traction.yaml)
set_tests_properties(${testName}_PlasticityJ2_2D_Traction
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
add_test(${testName}_PlasticityJ2_3D_Traction ${Albany.exe}
         PlasticityJ2_3D_Traction.yaml)
set_tests_properties(${testName}_PlasticityJ2_3D_Traction
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
