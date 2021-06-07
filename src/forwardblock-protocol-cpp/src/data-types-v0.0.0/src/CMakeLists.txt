
include(buffer/buffer.cmake)
include(algo/algo.cmake)
add_library(data_types_v0_0_0 validator.h configexception.h
        ${Buffer}
        ${CAlgo})
target_link_libraries(data_types_v0_0_0 PUBLIC CRYPTO)