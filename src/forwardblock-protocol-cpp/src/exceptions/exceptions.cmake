file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(EXCEPTIONS ${HEADER_FILES} ${SOURCE_FILES})