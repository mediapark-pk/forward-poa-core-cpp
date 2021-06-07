include_directories(include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(COMMON_COMPILE_FLAGS ENABLE_MODULE_RECOVERY ENABLE_MODULE_ECDH USE_FIELD_INV_BUILTIN USE_NUM_NONE USE_SCALAR_INV_BUILTIN)

add_executable(gen_context src/gen_context.c)
target_include_directories(gen_context PRIVATE ${CMAKE_SOURCE_DIR})

add_definitions(-DHAVE_CONFIG_H=1)

add_library(libsecp256k1 STATIC src/secp256k1.c)
target_compile_definitions(libsecp256k1 PUBLIC ${COMMON_COMPILE_FLAGS} ${COMPILE_FLAGS})
target_include_directories(libsecp256k1 PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_compile_options(libsecp256k1 PUBLIC ${COMPILE_OPTIONS})