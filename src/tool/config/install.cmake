execute_process(COMMAND cmake . -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
execute_process(COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/tool/config/config)
