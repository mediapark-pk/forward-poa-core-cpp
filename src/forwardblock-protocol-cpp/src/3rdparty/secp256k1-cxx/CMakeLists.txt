include_directories(libsecp256k1/include)

add_subdirectory(libsecp256k1)
add_subdirectory(cryptosecp)


add_library(secp256k1-cxx STATIC secp256k1-cxx.hpp secp256k1-cxx.cpp)

target_link_libraries(secp256k1-cxx PUBLIC libsecp256k1)
target_link_libraries(secp256k1-cxx PUBLIC secpcrypto)
