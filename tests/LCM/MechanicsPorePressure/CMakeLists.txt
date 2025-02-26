# Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputMechanicsSimple.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputMechanicsSimple.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PoroMaterialsSimple.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PoroMaterialsSimple.yaml COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR}Simple NAME)

# Create the test with this name and standard executable
if(NOT ALBANY_PARALLEL_ONLY)
  add_test(${testName}_Serial ${SerialAlbany.exe} inputMechanicsSimple.yaml)
  set_tests_properties(${testName}_Serial PROPERTIES LABELS
                                                     "LCM;Tpetra;Forward")
endif()

# Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputMechanics.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputMechanics.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PoroMaterials.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PoroMaterials.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/surface3ElemMesh.g
               ${CMAKE_CURRENT_BINARY_DIR}/surface3ElemMesh.g COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Copy Input file from source to binary dir
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/inputMechanicsLocalized.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/inputMechanicsLocalized.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PoroMaterialsLocalized.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PoroMaterialsLocalized.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/surface3ElemMesh.g
               ${CMAKE_CURRENT_BINARY_DIR}/surface3ElemMesh.g COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR}Localized NAME)

# Create the test with this name and standard executable
if(NOT ALBANY_PARALLEL_ONLY)
  add_test(${testName}_Serial ${SerialAlbany.exe} inputMechanicsLocalized.yaml)
  set_tests_properties(${testName}_Serial PROPERTIES LABELS
                                                     "LCM;Tpetra;Forward")
endif()

# Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputParallelFlow.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputParallelFlow.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PoroMaterialsLocalized.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PoroMaterialsLocalized.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/surface3ElemMesh.g
               ${CMAKE_CURRENT_BINARY_DIR}/surface3ElemMesh.g COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR}ParallelFlow NAME)

# Create the test with this name and standard executable
if(NOT ALBANY_PARALLEL_ONLY)
  add_test(${testName}_Serial ${SerialAlbany.exe} inputParallelFlow.yaml)
  set_tests_properties(${testName}_Serial PROPERTIES LABELS
                                                     "LCM;Tpetra;Forward")
endif()

# Copy Input file from source to binary dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputOrthogonalFlow.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/inputOrthogonalFlow.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PoroMaterialsLocalized.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/PoroMaterialsLocalized.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/surface3ElemMesh.g
               ${CMAKE_CURRENT_BINARY_DIR}/surface3ElemMesh.g COPYONLY)

# Name the test with the directory name
get_filename_component(testName ${CMAKE_CURRENT_SOURCE_DIR}OrthogonalFlow NAME)
