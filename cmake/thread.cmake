if(THREAD STREQUAL "")
    ERROR("No thread given!")

elseif(${THREAD} STREQUAL "Multiple")

elseif(${THREAD} STREQUAL "Serialized")

elseif(${THREAD} STREQUAL "Single")

else()
    ERROR("Unknown thread given!")
endif()

INFO("Thread Support: ${THREAD}")
