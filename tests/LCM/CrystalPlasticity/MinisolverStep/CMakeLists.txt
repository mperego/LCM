# Create a symbolic link for exodiff
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SEACAS_EXODIFF}
                        ${CMAKE_CURRENT_BINARY_DIR}/exodiff)

# Copy Input files from source to binary dir
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${runtest.cmake}
                        ${CMAKE_CURRENT_BINARY_DIR}/runtest.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_Specimen.g
               ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_Specimen.g COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_Newton.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_Newton.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_Newton_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_Newton_Material.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_Newton.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_Newton.exodiff COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_Newton.gold.e
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_Newton.gold.e COPYONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_NewtonLineSearch.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_NewtonLineSearch.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_NewtonLineSearch_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_NewtonLineSearch_Material.yaml
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_NewtonLineSearch.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_NewtonLineSearch.exodiff COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_NewtonLineSearch.gold.e
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_NewtonLineSearch.gold.e COPYONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_TrustRegion.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_TrustRegion.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_TrustRegion_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_TrustRegion_Material.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_TrustRegion.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_TrustRegion.exodiff COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_TrustRegion.gold.e
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_TrustRegion.gold.e COPYONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_ConjugateGradient.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_ConjugateGradient.yaml COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_ConjugateGradient_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_ConjugateGradient_Material.yaml
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_ConjugateGradient.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_ConjugateGradient.exodiff COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_ConjugateGradient.gold.e
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_ConjugateGradient.gold.e COPYONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_LineSearchRegularized.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_LineSearchRegularized.yaml
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_LineSearchRegularized_Material.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_LineSearchRegularized_Material.yaml
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_LineSearchRegularized.exodiff
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_LineSearchRegularized.exodiff
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MinisolverStep_LineSearchRegularized.gold.e
  ${CMAKE_CURRENT_BINARY_DIR}/MinisolverStep_LineSearchRegularized.gold.e
  COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# Add the tests add_test(CrystalPlasticity_MinisolverStep_ConjugateGradient
# python ../run_exodiff_test.py "${SerialAlbany.exe}"
# MinisolverStep_ConjugateGradient.yaml)
# add_test(CrystalPlasticity_MinisolverStep_LineSearchRegularized python
# ../run_exodiff_test.py "${SerialAlbany.exe}"
# MinisolverStep_LineSearchRegularized.yaml) test 1 - Newton
set(OUTFILE "MinisolverStep_Newton.e")
set(REF_FILE "MinisolverStep_Newton.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}_Newton
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=MinisolverStep_Newton -DTEST_ARGS=MinisolverStep_Newton.yaml
    -DMPIMNP=1 -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}_Newton
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
# test 2 - Newton Line Search
set(OUTFILE "MinisolverStep_NewtonLineSearch.e")
set(REF_FILE "MinisolverStep_NewtonLineSearch.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}_NewtonLineSearch
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=MinisolverStep_NewtonLineSearch
    -DTEST_ARGS=MinisolverStep_NewtonLineSearch.yaml -DMPIMNP=1
    -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}_NewtonLineSearch
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
# test 3 - Trust Region
set(OUTFILE "MinisolverStep_TrustRegion.e")
set(REF_FILE "MinisolverStep_TrustRegion.gold.e")
add_test(
  NAME CrystalPlasticity_${testName}_TrustRegion
  COMMAND
    ${CMAKE_COMMAND} "-DTEST_PROG=${SerialAlbany.exe}"
    -DTEST_NAME=MinisolverStep_TrustRegion
    -DTEST_ARGS=MinisolverStep_TrustRegion.yaml -DMPIMNP=1
    -DSEACAS_EXODIFF=${SEACAS_EXODIFF} -DREF_FILENAME=${REF_FILE}
    -DOUTPUT_FILENAME=${OUTFILE} -DDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P
    ${runtest.cmake})
set_tests_properties(CrystalPlasticity_${testName}_TrustRegion
                     PROPERTIES LABELS "LCM;Tpetra;Forward")
