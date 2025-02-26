# Create a symbolic link for exodiff
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SEACAS_EXODIFF}
                        ${CMAKE_CURRENT_BINARY_DIR}/exodiff)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${runtest.cmake}
                        ${CMAKE_CURRENT_BINARY_DIR}/runtest.cmake)

# Copy Input files from source to binary dir

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SingleSlipSaturation.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/SingleSlipSaturation.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SingleSlip.g
               ${CMAKE_CURRENT_BINARY_DIR}/SingleSlip.g COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/SingleSlipSaturation_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/SingleSlipSaturation_Material.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/SingleSlipSaturation.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/SingleSlipSaturation.exodiff COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SingleSlipSaturation.gold.e
               ${CMAKE_CURRENT_BINARY_DIR}/SingleSlipSaturation.gold.e COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# Add the tests
set(OUTFILE "SingleSlipSaturation.e")
set(REF_FILE "SingleSlipSaturation.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=SingleSlipSaturation -DTEST_ARGS=SingleSlipSaturation.yaml
    -DMPIMNP=1 -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
