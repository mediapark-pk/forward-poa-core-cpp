add_subdirectory(forwardblock-protocol-cpp)
include(protocol/protocol.cmake)
add_executable(helloworld main.cpp ${PROTOCOL})
target_link_libraries(helloworld forward_protocol)