# Create a symbolic link for exodiff
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SEACAS_EXODIFF}
                        ${CMAKE_CURRENT_BINARY_DIR}/exodiff)
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SEACAS_EPU}
# ${CMAKE_CURRENT_BINARY_DIR}/epu)

# Copy Input files from source to binary dir
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${runtest.cmake}
                        ${CMAKE_CURRENT_BINARY_DIR}/runtest.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/blockDiffusion.e
               ${CMAKE_CURRENT_BINARY_DIR}/blockDiffusion.e COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/blockDiffusionStab.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/blockDiffusionStab.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/materialsScaledPlasticity.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/materialsScaledPlasticity.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/blockDiffusionStab.exodiff
               ${CMAKE_CURRENT_BINARY_DIR}/blockDiffusionStab.exodiff COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/blockDiffusionStab.gold.e
               ${CMAKE_CURRENT_BINARY_DIR}/blockDiffusionStab.gold.e COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# Add the tests Big diffs in parallel
set(OUTFILE "blockDiffusionStab.e")
set(REF_FILE "blockDiffusionStab.gold.e")
add_test(
  NAME MechanicsWithHydrogenBar_${testName}
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=blockDiffusionStab -DTEST_ARGS=blockDiffusionStab.yaml
    -DMPIMNP=1 -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(MechanicsWithHydrogenBar_${testName}
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
