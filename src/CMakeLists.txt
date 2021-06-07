add_subdirectory(forwardblock-protocol-cpp)
include(protocol/protocol.cmake)
add_executable(helloworld main.cpp)
target_link_libraries(helloworld protocol)