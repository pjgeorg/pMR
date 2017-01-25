if(CLUSTER STREQUAL "")
    ERROR("No target cluster given!")

elseif(${CLUSTER} STREQUAL "iDataCool")
    set(PROVIDER.NULL ON)
    set(PROVIDER.SELF ON)
    set(PROVIDER.CMA ON)
    set(PROVIDER.VERBS ON)
    set(ALLREDUCE "RECURSIVEDOUBLING")

elseif(${CLUSTER} STREQUAL "MPI")
    set(PROVIDER.MPI ON)
    set(ALLREDUCE "RECURSIVEDOUBLING")

elseif(${CLUSTER} STREQUAL "QPACE2")
    set(PROVIDER.NULL ON)
    set(PROVIDER.SELF ON)
    set(PROVIDER.SCIF ON)
    set(PROVIDER.VERBS ON)
    set(VERBS.MTU 2048 CACHE STRING "Set MTU size.")
    set(VERBS.INLINE 256 CACHE STRING "Set max inline data size.")
    set(ALLREDUCE "RECURSIVEDOUBLING")
    if(QPACE2.WARN.TOPOLOGY)
        add_definitions(-DQPACE2_WARN_TOPOLOGY)
    endif()

elseif(${CLUSTER} STREQUAL "QPACE3")
    set(PROVIDER.NULL ON)
    set(PROVIDER.SELF ON)
    set(PROVIDER.OFI ON)
    set(ALLREDUCE "RECURSIVEDOUBLING")

elseif(${CLUSTER} STREQUAL "QPACEB")
    set(PROVIDER.NULL ON)
    set(PROVIDER.SELF ON)
    set(PROVIDER.VERBS ON)
    set(ALLREDUCE "RECURSIVEDOUBLING")

elseif(${CLUSTER} STREQUAL "SHM")
    set(PROVIDER.NULL ON)
    set(PROVIDER.SELF ON)
    set(PROVIDER.CMA ON)
    set(ALLREDUCE "RECURSIVEDOUBLING")

else()
    ERROR("Unknown target cluster given!")
endif()

INFO("Cluster: ${CLUSTER}")
