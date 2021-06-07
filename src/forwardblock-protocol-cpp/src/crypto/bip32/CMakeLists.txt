add_library(bip32-cxx STATIC bip32.cpp extendedkey.cpp masterkey.cpp)

target_include_directories(bip32-cxx PRIVATE ../../)

target_link_libraries(bip32-cxx PUBLIC secp256k1-cxx)
#target_link_libraries(bip32-cxx PRIVATE crypto)
