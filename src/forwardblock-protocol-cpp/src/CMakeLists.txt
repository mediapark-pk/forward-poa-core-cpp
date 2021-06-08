add_subdirectory(3rdparty/)
include(utils/utils.cmake)
add_subdirectory(crypto)
add_subdirectory(data-types-v0.0.0)

include_directories(crypto)
include_directories(data-types-v0.0.0/src)

include(exceptions/exceptions.cmake)
include(accounts/account.cmake)
include(transactions/transaction.cmake)
include(utils/utils.cmake)
include(base58/base58.cmake)
include(keypair/keypair.cmake)
include(blocks/block.cmake)



add_executable(main main.cpp protocolconstants.h config.h config.cpp  ${Utils} validator.h validator.cpp ${Base58} ${ACCOUNTS}
            ${KEYPAIR} ${EXCEPTIONS}  ${TRANSACTION} abstractprotocolchain.h abstractprotocolchain.cpp ${BLOCK})
target_link_libraries(main PUBLIC CRYPTO spdlog secp256k1-cxx data_types_v0_0_0 )
target_compile_features(main PRIVATE cxx_std_17)


add_library(forward_protocol main.cpp protocolconstants.h config.h config.cpp  ${Utils} validator.h validator.cpp ${Base58} ${ACCOUNTS}
        ${KEYPAIR} ${EXCEPTIONS}  ${TRANSACTION} abstractprotocolchain.h abstractprotocolchain.cpp ${BLOCK})
target_link_libraries(forward_protocol PUBLIC CRYPTO spdlog secp256k1-cxx data_types_v0_0_0 )
target_compile_features(forward_protocol PRIVATE cxx_std_17)