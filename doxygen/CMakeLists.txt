find_package(Doxygen)
if(DOXYGEN_FOUND)
    string(TOLOWER ${BACKEND} BACKEND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.in
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen
        WORKING_DIRECTORY ${pMR_SOURCE_DIR}
        )
else()
    message(WARNING "Doxygen not found!")
endif(DOXYGEN_FOUND)
