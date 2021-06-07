include_directories(../3rdparty/libpack)

set(UTILS_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/mathencodings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mathencodings.h
        ${CMAKE_CURRENT_LIST_DIR}/strencodings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/strencodings.h
        ${CMAKE_CURRENT_LIST_DIR}/time.cpp
        ${CMAKE_CURRENT_LIST_DIR}/time.h
        ${CMAKE_CURRENT_LIST_DIR}/utilstring.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utilstring.h
        ${CMAKE_CURRENT_LIST_DIR}/utils.h
        ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/bcmath/bcmath.h
        ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/bcmath/bcmath.cpp
        )

set(Utils ${UTILS_SOURCES})

