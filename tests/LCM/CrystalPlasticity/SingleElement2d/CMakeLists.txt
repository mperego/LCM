# Create a symbolic link for exodiff
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SEACAS_EXODIFF}
                        ${CMAKE_CURRENT_BINARY_DIR}/exodiff)

# Copy Input files from source to binary dir
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${runtest.cmake}
                        ${CMAKE_CURRENT_BINARY_DIR}/runtest.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ElasticShear2d.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/ElasticShear2d.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/CrystalElasticity2d_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/CrystalElasticity2d_Material.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ElasticShear2d.exodiff
               ${CMAKE_CURRENT_BINARY_DIR}/ElasticShear2d.exodiff COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ElasticShear2d.gold.e
               ${CMAKE_CURRENT_BINARY_DIR}/ElasticShear2d.gold.e COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PlasticShear2d.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PlasticShear2d.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/CrystalPlasticity2d_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/CrystalPlasticity2d_Material.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PlasticShear2d.exodiff
               ${CMAKE_CURRENT_BINARY_DIR}/PlasticShear2d.exodiff COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PlasticShear2d.gold.e
               ${CMAKE_CURRENT_BINARY_DIR}/PlasticShear2d.gold.e COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# Add the tests test 1 - Elastic shear
set(OUTFILE "ElasticShear2d.e")
set(REF_FILE "ElasticShear2d.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}_ElasticShear2d
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=ElasticShear2d -DTEST_ARGS=ElasticShear2d.yaml -DMPIMNP=1
    -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}_ElasticShear2d
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
# test 2 - Plastic shear
set(OUTFILE "PlasticShear2d.e")
set(REF_FILE "PlasticShear2d.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}_PlasticShear2d
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=PlasticShear2d -DTEST_ARGS=PlasticShear2d.yaml -DMPIMNP=1
    -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}_PlasticShear2d
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
