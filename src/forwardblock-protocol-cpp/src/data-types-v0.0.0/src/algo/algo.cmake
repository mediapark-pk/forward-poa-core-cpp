include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

file(GLOB algo_sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB algo_headers ${CMAKE_CURRENT_LIST_DIR}/*.h)

set(CAlgo ${algo_sources} ${algo_headers})
