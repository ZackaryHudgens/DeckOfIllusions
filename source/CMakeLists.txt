find_package(UrsineEngine REQUIRED)

file(GLOB DECK_SOURCES *.cpp)

add_executable(deck-of-illusions ${DECK_SOURCES})

target_link_libraries(deck-of-illusions PUBLIC
                      ${URSINE_LIBRARIES})
target_include_directories(deck-of-illusions PUBLIC
                           ${URSINE_INCLUDE_DIRS})

install(TARGETS deck-of-illusions DESTINATION ${INSTALL_DIR}/deck-of-illusions)
install(DIRECTORY resources DESTINATION ${INSTALL_DIR}/deck-of-illusions)
