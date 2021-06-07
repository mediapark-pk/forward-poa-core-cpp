add_library(bip39-cxx bip39.cpp mnemonic.h mnemonic.cpp wordlist.cpp)

#target_link_libraries(bip39-cxx PRIVATE crypto)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/english.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/french.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/spanish.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/italian.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)

