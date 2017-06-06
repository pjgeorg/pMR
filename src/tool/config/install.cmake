get_filename_component(INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
execute_process(COMMAND cmake . -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
execute_process(COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
