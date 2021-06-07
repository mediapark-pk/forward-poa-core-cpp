include_directories(${CMAKE_CURRENT_LIST_DIR}/../)
include_directories(${CMAKE_SOURCE_DIR}/src/3rdParty)


#file(GLOB buffer_base16_header ${CMAKE_CURRENT_LIST_DIR}/base16/*.h)
#file(GLOB buffer_base16_sources ${CMAKE_CURRENT_LIST_DIR}/base16/*.cpp)
#file(GLOB buffer_binary_sources ${CMAKE_CURRENT_LIST_DIR}/binary/*.h)
#file(GLOB buffer_binary_sources ${CMAKE_CURRENT_LIST_DIR}/binary/*.cpp)

file(GLOB buffer_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB buffer_headers ${CMAKE_CURRENT_LIST_DIR}/*.h)

set(Buffer ${buffer_sources} ${buffer_headers}) #${buffer_base16_header}
#        ${buffer_base16_sources} ${buffer_binary_sources} ${buffer_binary_sources})
